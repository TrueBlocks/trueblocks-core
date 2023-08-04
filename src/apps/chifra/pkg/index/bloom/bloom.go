package bloom

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"encoding/binary"
	"errors"
	"fmt"
	"io"
	"os"
	"unsafe"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
)

const (
	// The number of bytes in a single BloomByte structure
	BLOOM_WIDTH_IN_BYTES = (BLOOM_WIDTH_IN_BITS / 8)
	// The number of bits in a single BloomByte structure
	BLOOM_WIDTH_IN_BITS = (1048576)
	// The maximum number of addresses to add to a BloomBytes before creating a new one
	MAX_ADDRS_IN_BLOOM = 50000
)

// BloomBytes store the actual bits of the bloom filter. There is at least one but likely more BloomBytes contained in
// each ChunkBloom. The NInserted value, which is for statistical purposes only, records the number of addresses
// inserted in the Bytes.
type BloomBytes struct {
	NInserted uint32 // Do not change the size of this field, it's stored on disc
	Bytes     []byte
}

type BloomHeader struct {
	Magic uint16    `json:"magic"`
	Hash  base.Hash `json:"hash"`
}

// ChunkBloom structures contain an array of BloomBytes each BLOOM_WIDTH_IN_BYTES wide. A new BloomBytes is added to
// the ChunkBloom when around MAX_ADDRS_IN_BLOOM addresses has been added. These Adaptive Bloom Filters allow us to
// maintain a near-constant false-positive rate at the expense of slightly larger bloom filters than might be expected.
type ChunkBloom struct {
	File       *os.File
	SizeOnDisc int64
	Range      base.FileRange
	HeaderSize int64
	Header     BloomHeader
	Count      uint32 // Do not change the size of this field, it's stored on disc
	Blooms     []BloomBytes
}

func (bl *ChunkBloom) String() string {
	nInserted := uint32(0)
	for i := uint32(0); i < bl.Count; i++ {
		nInserted += bl.Blooms[i].NInserted
	}
	return fmt.Sprintf("%s\t%d\t%d\t%d", bl.Range, bl.Count, BLOOM_WIDTH_IN_BYTES, nInserted)
}

// NewChunkBloom returns a newly initialized bloom filter. The bloom filter's file pointer is open (if there
// have been no errors) and its header data has been read into memory. The array has been created with
// enough space for Count blooms but has not been read from disc. The file remains open for reading (if
// there is no error) and is positioned at the start of the file.
func NewChunkBloom(path string) (bl ChunkBloom, err error) {
	if !file.FileExists(path) {
		return bl, errors.New("required bloom file (" + path + ") missing")
	}

	bl.SizeOnDisc = file.FileSize(path)
	bl.Range, err = base.RangeFromFilenameE(path)
	if err != nil {
		return
	}

	bl.File, err = os.OpenFile(path, os.O_RDONLY, 0644)
	if err != nil {
		return
	}

	_, _ = bl.File.Seek(0, io.SeekStart)        // already true, but can't hurt
	if err = bl.ReadBloomHeader(); err != nil { // Note that it may not find a header, but it leaves the file pointer pointing to the count
		return
	}

	if err = binary.Read(bl.File, binary.LittleEndian, &bl.Count); err != nil {
		return
	}
	bl.Blooms = make([]BloomBytes, 0, bl.Count)
	_, _ = bl.File.Seek(int64(bl.HeaderSize), io.SeekStart) // Point to the start of Count

	return
}

// Close closes the file if it's opened
func (bl *ChunkBloom) Close() {
	if bl.File != nil {
		bl.File.Close()
		bl.File = nil
	}
}

// ReadBloom reads the entire contents of the bloom filter
func (bl *ChunkBloom) ReadBloom(fileName string) (err error) {
	bl.Range, err = base.RangeFromFilenameE(fileName)
	if err != nil {
		return err
	}

	bl.File, err = os.OpenFile(fileName, os.O_RDONLY, 0644)
	if err != nil {
		return err
	}
	defer func() {
		bl.File.Close()
		bl.File = nil
	}()

	_, _ = bl.File.Seek(0, io.SeekStart)        // already true, but can't hurt
	if err = bl.ReadBloomHeader(); err != nil { // Note that it may not find a header, but it leaves the file pointer pointing to the count
		return err
	}

	if err = binary.Read(bl.File, binary.LittleEndian, &bl.Count); err != nil {
		return err
	}

	bl.Blooms = make([]BloomBytes, bl.Count)
	for i := uint32(0); i < bl.Count; i++ {
		if err = binary.Read(bl.File, binary.LittleEndian, &bl.Blooms[i].NInserted); err != nil {
			return err
		}

		bl.Blooms[i].Bytes = make([]byte, BLOOM_WIDTH_IN_BYTES)
		if err = binary.Read(bl.File, binary.LittleEndian, &bl.Blooms[i].Bytes); err != nil {
			return err
		}
	}

	return nil
}

var ErrInvalidBloomMagic = errors.New("invalid magic number in bloom header")
var ErrInvalidBloomHash = errors.New("invalid hash in bloom header")

func (bl *ChunkBloom) ReadBloomHeader() error {
	bl.HeaderSize = 0 // already true, but it makes it explicit
	err := binary.Read(bl.File, binary.LittleEndian, &bl.Header)
	if err != nil {
		bl.Header = BloomHeader{}
		_, _ = bl.File.Seek(0, io.SeekStart)
		return err
	}

	if bl.Header.Magic != file.SmallMagicNumber {
		// This is an unversioned bloom filter, set back to start of file
		bl.Header = BloomHeader{}
		_, _ = bl.File.Seek(0, io.SeekStart)
		return ErrInvalidBloomMagic
	}

	bl.HeaderSize = int64(unsafe.Sizeof(bl.Header))
	if bl.Header.Hash.Hex() != unchained.HeaderMagicHash {
		return ErrInvalidBloomHash
	}

	return nil
}

// AddToSet adds an address to a bloom filter
func (bl *ChunkBloom) AddToSet(addr base.Address) {
	if len(bl.Blooms) == 0 {
		bl.Blooms = append(bl.Blooms, BloomBytes{})
		bl.Blooms[bl.Count].Bytes = make([]byte, BLOOM_WIDTH_IN_BYTES)
		bl.Count++
	}

	loc := len(bl.Blooms) - 1
	bits := bl.WhichBits(addr)
	for _, bit := range bits {
		which := (bit / 8)
		whence := (bit % 8)
		index := BLOOM_WIDTH_IN_BYTES - which - 1
		mask := uint8(1 << whence)
		bl.Blooms[loc].Bytes[index] |= mask
	}
	bl.Blooms[loc].NInserted++

	if bl.Blooms[loc].NInserted > MAX_ADDRS_IN_BLOOM {
		bl.Blooms = append(bl.Blooms, BloomBytes{})
		bl.Blooms[bl.Count].Bytes = make([]byte, BLOOM_WIDTH_IN_BYTES)
		bl.Count++
	}
}

// WhichBits returns the five bits calculated from an address used to determine if the address is
// in the bloom filter. We get the five bits by cutting the 20-byte address into five equal four-byte
// parts, turning those four bytes into an 32-bit integer modulo the width of a bloom array item.
func (bl *ChunkBloom) WhichBits(addr base.Address) (bits [5]uint32) {
	slice := addr.Bytes()
	if len(slice) != 20 {
		logger.Fatal("address is not 20 bytes long - should not happen")
	}

	cnt := 0
	for i := 0; i < len(slice); i += 4 {
		bytes := slice[i : i+4]
		bits[cnt] = (binary.BigEndian.Uint32(bytes) % uint32(BLOOM_WIDTH_IN_BITS))
		cnt++
	}

	return
}

func (bl *ChunkBloom) GetStats() (nBlooms uint64, nInserted uint64, nBitsLit uint64, nBitsNotLit uint64, sz uint64, bitsLit []uint64) {
	bitsLit = []uint64{}
	sz += 4
	nBlooms = uint64(bl.Count)
	for _, bf := range bl.Blooms {
		nInserted += uint64(bf.NInserted)
		sz += 4 + uint64(len(bf.Bytes))
		for bitPos := 0; bitPos < len(bf.Bytes)*8; bitPos++ {
			tester := bitChecker{bit: uint32(bitPos), bytes: bf.Bytes}
			if bl.isBitLit(&tester) {
				nBitsLit++
				bitsLit = append(bitsLit, uint64(bitPos))
			} else {
				nBitsNotLit++
				// fmt.Printf("%d", b)
			}
		}
	}
	return
}
