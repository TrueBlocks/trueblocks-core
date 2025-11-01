package index

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"encoding/binary"
	"errors"
	"io"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/ranges"
)

const (
	// The number of bytes in a single BloomByte structure
	BLOOM_WIDTH_IN_BYTES = (BLOOM_WIDTH_IN_BITS / 8)
	// The number of bits in a single BloomByte structure
	BLOOM_WIDTH_IN_BITS = (1048576)
	// The maximum number of addresses to add to a bloomBytes before creating a new one
	MAX_ADDRS_IN_BLOOM = 50000
)

// bloomBytes store the actual bits of the bloom filter. There is at least one but likely more bloomBytes contained in
// each Bloom. The NInserted value, which is for statistical purposes only, records the number of addresses
// inserted in the Bytes.
type bloomBytes struct {
	NInserted uint32 // Do not change the size of this field, it's stored on disc
	Bytes     []byte
}

type bloomHeader struct {
	Magic uint16    `json:"magic"`
	Hash  base.Hash `json:"hash"`
}

// Bloom structures contain an array of bloomBytes each BLOOM_WIDTH_IN_BYTES wide. A new bloomBytes is added to
// the Bloom when around MAX_ADDRS_IN_BLOOM addresses has been added. These Adaptive Bloom Filters allow us to
// maintain a near-constant false-positive rate at the expense of slightly larger bloom filters than might be expected.
type Bloom struct {
	File       *os.File
	SizeOnDisc int64
	Range      ranges.FileRange
	HeaderSize int64
	Header     bloomHeader
	Count      uint32 // Do not change the size of this field, it's stored on disc
	Blooms     []bloomBytes
}

// ReadBloomMetadata reads only the header, count, and nInserted values without loading bloom data
func ReadBloomMetadata(path string, check, verbose bool) (header bloomHeader, count uint32, totalInserted uint64, err error) {
	if !file.FileExists(path) {
		err = errors.New("required bloom file (" + path + ") missing")
		return
	}

	var bl Bloom
	if bl.File, err = os.OpenFile(path, os.O_RDONLY, 0644); err != nil {
		return
	}
	defer func() {
		bl.File.Close()
	}()

	// Read header
	_, _ = bl.File.Seek(0, io.SeekStart)
	if err = bl.readHeader(check); err != nil {
		return
	}
	header = bl.Header

	// Read count
	if err = binary.Read(bl.File, binary.LittleEndian, &count); err != nil {
		return
	}

	// Read only the nInserted values (skip the actual bloom bytes)
	totalInserted = 0
	if verbose {
		for i := uint32(0); i < count; i++ {
			var nInserted uint32
			if err = binary.Read(bl.File, binary.LittleEndian, &nInserted); err != nil {
				return
			}
			totalInserted += uint64(nInserted)

			// Skip the bloom bytes (BLOOM_WIDTH_IN_BYTES = 131072)
			if _, err = bl.File.Seek(BLOOM_WIDTH_IN_BYTES, io.SeekCurrent); err != nil {
				return
			}
		}
	}
	return
}

// OpenBloom returns a newly initialized bloom filter. The bloom filter's file pointer is open (if there
// have been no errors) and its header data has been read into memory. The array has been created with
// enough space for Count blooms but has not been read from disc. The file remains open for reading (if
// there is no error) and is positioned at the start of the file.
func OpenBloom(path string, check bool) (Bloom, error) {
	var err error
	var bl Bloom

	if !file.FileExists(path) {
		return bl, errors.New("required bloom file (" + path + ") missing")
	}

	bl.SizeOnDisc = file.FileSize(path)
	if bl.Range, err = ranges.RangeFromFilenameE(path); err != nil {
		return bl, err
	}

	if bl.File, err = os.OpenFile(path, os.O_RDONLY, 0644); err != nil {
		return bl, err
	}

	_, _ = bl.File.Seek(0, io.SeekStart)        // already true, but can't hurt
	if err = bl.readHeader(check); err != nil { // Note that it may not find a header, but it leaves the file pointer pointing to the count
		return bl, err
	}

	if err = binary.Read(bl.File, binary.LittleEndian, &bl.Count); err != nil {
		return bl, err
	}

	bl.Blooms = make([]bloomBytes, 0, bl.Count)
	_, _ = bl.File.Seek(int64(bl.HeaderSize), io.SeekStart) // Point to the start of Count
	return bl, nil
}

// Close closes the file if it's opened
func (bl *Bloom) Close() {
	if bl.File != nil {
		bl.File.Close()
		bl.File = nil
	}
}

// InsertAddress adds an address to the bloom filter.
func (bl *Bloom) InsertAddress(addr base.Address) {

	// Check and initialize if empty.
	if len(bl.Blooms) == 0 {
		bl.Blooms = append(bl.Blooms, bloomBytes{})
		bl.Blooms[bl.Count].Bytes = make([]byte, BLOOM_WIDTH_IN_BYTES)
		bl.Count++
	}

	// Get location and convert address to bits.
	loc := len(bl.Blooms) - 1
	bits := bl.addressToBits(addr)

	// Set address bits in bloom.
	for _, bit := range bits {
		which := (bit / 8)
		whence := (bit % 8)
		index := BLOOM_WIDTH_IN_BYTES - which - 1
		mask := uint8(1 << whence)
		bl.Blooms[loc].Bytes[index] |= mask
	}

	// Update insert count and check for overflow.
	bl.Blooms[loc].NInserted++
	if bl.Blooms[loc].NInserted > MAX_ADDRS_IN_BLOOM {
		bl.Blooms = append(bl.Blooms, bloomBytes{})
		bl.Blooms[bl.Count].Bytes = make([]byte, BLOOM_WIDTH_IN_BYTES)
		bl.Count++
	}
}

// addressToBits extracts five bits from a 20-byte address to determine its presence in the bloom filter.
// It divides the address into five 4-byte segments, converts each to a 32-bit integer, and then takes the modulo
// with the bloom array item width.
func (bl *Bloom) addressToBits(addr base.Address) (bits [5]uint32) {

	// Convert address to byte slice.
	slice := addr.Bytes()

	// Validate address length.
	if len(slice) != 20 {
		logger.ShouldNotHappen("invalid address length in addressToBits")
	}

	// Split address into five segments and calculate corresponding bits.
	for i, cnt := 0, 0; i < len(slice); i += 4 {
		bytes := slice[i : i+4]
		bits[cnt] = (binary.BigEndian.Uint32(bytes) % uint32(BLOOM_WIDTH_IN_BITS))
		cnt++
	}

	return
}
