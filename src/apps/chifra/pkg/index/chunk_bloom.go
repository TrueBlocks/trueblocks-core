// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package index

import (
	"encoding/binary"
	"errors"
	"fmt"
	"io"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/ethereum/go-ethereum/common"
)

// ChunkBloom structures contain an array of BloomBytes each BLOOM_WIDTH_IN_BYTES wide. A new BloomBytes is added to
// the ChunkBloom when around MAX_ADDRS_IN_BLOOM addresses has been added. These Adaptive Bloom Filters allow us to
// maintain a near-constant false-positive rate at the expense of slightly larger bloom filters than might be expected.
type ChunkBloom struct {
	File   *os.File
	Size   int64
	Range  cache.FileRange
	Count  uint32 // Do not change the size of this field, it's stored on disc
	Blooms []BloomBytes
}

// BloomBytes store the actual bits of the bloom filter. There is at least one but likely more BloomBytes contained in
// each ChunkBloom. The NInserted value, which is for statistical purposes only, records the number of addresses
// inserted in the Bytes.
type BloomBytes struct {
	NInserted uint32 // Do not change the size of this field, it's stored on disc
	Bytes     []byte
}

const (
	// The number of bytes in a single BloomByte structure
	BLOOM_WIDTH_IN_BYTES = (BLOOM_WIDTH_IN_BITS / 8)
	// The number of bits in a single BloomByte structure
	BLOOM_WIDTH_IN_BITS = (1048576)
	// The maximum number of addresses to add to a BloomBytes before creating a new one
	MAX_ADDRS_IN_BLOOM = 50000
)

// NewChunkBloom returns a newly initialized bloom filter. The bloom filter's file pointer is open (if there
// have been no errors) and its header data has been read into memory. The array has been created with
// enough space for Count blooms but has not been read from disc. The file remains open for reading (if
// there is no error) and is positioned at the start of the file.
func NewChunkBloom(path string) (bloom ChunkBloom, err error) {
	if !file.FileExists(path) {
		return bloom, errors.New("required bloom file (" + path + ") not found")
	}

	bloom.Size = file.FileSize(path)

	bloom.Range, err = cache.RangeFromFilename(path)
	if err != nil {
		return
	}

	bloom.File, err = os.OpenFile(path, os.O_RDONLY, 0644)
	if err != nil {
		return
	}

	err = binary.Read(bloom.File, binary.LittleEndian, &bloom.Count)
	if err != nil {
		return
	}
	bloom.Blooms = make([]BloomBytes, 0, bloom.Count)
	bloom.File.Seek(0, io.SeekStart)

	return
}

// Close closes the file if it's opened
func (bloom *ChunkBloom) Close() {
	if bloom.File != nil {
		bloom.File.Close()
		bloom.File = nil
	}
}

// ToBloomPath returns a path pointing to the bloom filter given either a path to itself or its associated index data
func ToBloomPath(pathIn string) string {
	if strings.HasSuffix(pathIn, ".bloom") {
		return pathIn
	}

	ret := strings.Replace(pathIn, ".bin", ".bloom", -1)
	ret = strings.Replace(ret, "/finalized/", "/blooms/", -1)
	return ret
}

//---------------------------------------------------------------------------
func ReadBloom(bloom *ChunkBloom, fileName string) (err error) {
	bloom.Range, err = cache.RangeFromFilename(fileName)
	if err != nil {
		return err
	}

	bloom.File, err = os.OpenFile(fileName, os.O_RDONLY, 0644)
	if err != nil {
		return err
	}
	defer func() {
		bloom.File.Close()
		bloom.File = nil
	}()

	err = binary.Read(bloom.File, binary.LittleEndian, &bloom.Count)
	if err != nil {
		return err
	}

	bloom.Blooms = make([]BloomBytes, bloom.Count)
	for i := uint32(0); i < bloom.Count; i++ {
		// fmt.Println("nBlooms:", bloom.Count)
		err = binary.Read(bloom.File, binary.LittleEndian, &bloom.Blooms[i].NInserted)
		if err != nil {
			return err
		}
		// fmt.Println("nInserted:", bloom.Blooms[i].NInserted)
		bloom.Blooms[i].Bytes = make([]byte, BLOOM_WIDTH_IN_BYTES)
		err = binary.Read(bloom.File, binary.LittleEndian, &bloom.Blooms[i].Bytes)
		if err != nil {
			return err
		}
	}

	return nil
}

func (bloom *ChunkBloom) Display(verbose int) {
	var bytesPerLine = (2048 / 16)
	if verbose > 0 {
		if verbose > 4 {
			bytesPerLine = 128
		} else {
			bytesPerLine = 32
		}
	}

	nInserted := uint32(0)
	for i := uint32(0); i < bloom.Count; i++ {
		nInserted += bloom.Blooms[i].NInserted
	}
	fmt.Println("range:", bloom.Range)
	fmt.Println("nBlooms:", bloom.Count)
	fmt.Println("byteWidth:", BLOOM_WIDTH_IN_BYTES)
	fmt.Println("nInserted:", nInserted)
	if verbose > 0 {
		for i := uint32(0); i < bloom.Count; i++ {
			for j := 0; j < len(bloom.Blooms[i].Bytes); j++ {
				if (j % bytesPerLine) == 0 {
					if j != 0 {
						fmt.Println()
					}
				}
				ch := bloom.Blooms[i].Bytes[j]
				str := fmt.Sprintf("%08b", ch)
				fmt.Printf("%s ", strings.Replace(str, "0", ".", -1))
			}
		}
		fmt.Println()
	}
}

// AddToSet adds an address to a bloom filter
func (bloom *ChunkBloom) AddToSet(addr common.Address) {
	if len(bloom.Blooms) == 0 {
		bloom.Blooms = append(bloom.Blooms, BloomBytes{})
		bloom.Blooms[0].Bytes = make([]byte, BLOOM_WIDTH_IN_BYTES)
	}

	loc := len(bloom.Blooms) - 1
	bits := WhichBits(addr)
	for _, bit := range bits {
		which := (bit / 8)
		whence := (bit % 8)
		index := BLOOM_WIDTH_IN_BYTES - which - 1
		mask := uint8(1 << whence)
		bloom.Blooms[loc].Bytes[index] |= mask
	}
	bloom.Blooms[loc].NInserted++

	if bloom.Blooms[loc].NInserted > MAX_ADDRS_IN_BLOOM {
		bloom.Blooms = append(bloom.Blooms, BloomBytes{})
	}
}

// writeBloom
//     lockSection();
//     CArchive output(WRITING_ARCHIVE);
//     if (!output.Lock(fileName, modeWriteCreate, LOCK_NOWAIT)) {
//         unlockSection();
//         return false;
//     }
//     output.Write((uint32_t)blooms.size());
//     for (auto bloom : blooms) {
//         output.Write(bloom.nInserted);
//         output.Write(bloom.bits, sizeof(uint8_t), BLOOM_WIDTH_IN_BYTES);
//     }
//     output.Release();
//     unlockSection();
//     return true;
// }
