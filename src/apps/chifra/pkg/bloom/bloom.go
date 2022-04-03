package bloomPkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"encoding/binary"
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/ethereum/go-ethereum/common"
)

type bloomBytes struct {
	NInserted uint32
	Bytes     []byte
}

type BloomFilter struct {
	Range  cache.FileRange
	Count  uint32
	Blooms []bloomBytes
}

const (
	BLOOM_WIDTH_IN_BITS  = (1048576)
	BLOOM_WIDTH_IN_BYTES = (BLOOM_WIDTH_IN_BITS / 8)
	MAX_ADDRS_IN_BLOOM   = 50000
)

func NewBloomFilter() BloomFilter {
	var ret BloomFilter
	ret.Blooms = make([]bloomBytes, 0, 1)
	ret.Count = 1
	return ret
}

func bitsToLight(addr common.Address) (parts [5][]byte, vals [5]uint32, bits [5]uint32) {
	cnt := 0
	slice := addr.Bytes()
	for i := 0; i < len(slice); i += 4 {
		end := i + 4
		if end > len(slice) {
			end = len(slice)
		}
		bytes := slice[i:end]
		parts[cnt] = bytes
		vals[cnt] = binary.BigEndian.Uint32(bytes)
		bits[cnt] = (vals[cnt] % uint32(BLOOM_WIDTH_IN_BITS))
		cnt++
	}
	return
}

//----------------------------------------------------------------------
func (bloom *BloomFilter) getStats() (nBlooms uint64, nInserted uint64, nBitsLit uint64, nBitsNotLit uint64, sz uint64, bitsLit []uint64) {
	bitsLit = []uint64{}
	sz += 4
	nBlooms = uint64(bloom.Count)
	for _, bf := range bloom.Blooms {
		nInserted += uint64(bf.NInserted)
		sz += 4 + uint64(len(bf.Bytes))
		for bitPos := 0; bitPos < len(bf.Bytes)*8; bitPos++ {
			if IsBitLit(uint32(bitPos), bf.Bytes) {
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

//----------------------------------------------------------------------
func (bb *bloomBytes) isInBloomBytes(addr common.Address) bool {
	_, _, bitsLitInAddr := bitsToLight(addr)
	// fmt.Println("Checking address", addr.Hex(), bitsLitInAddr)
	for _, bit := range bitsLitInAddr {
		// fmt.Println("\tTesting:", bit, IsBitLit(bit, bb.Bytes))
		if !IsBitLit(bit, bb.Bytes) {
			// fmt.Println("\t\tMISS")
			return false
			// } else {
			// 	fmt.Println("\t\tHIT")
		}
	}
	return true
}

//----------------------------------------------------------------------
func (bloom *BloomFilter) IsMemberOf(addr common.Address) bool {
	// _, _, _, _, _, bitsLit := bloom.getStats()
	// fmt.Println("\n------------------------------------\nbitsLit:", bitsLit)
	for _, bb := range bloom.Blooms {
		if bb.isInBloomBytes(addr) {
			return true
		}
	}
	return false
}

//---------------------------------------------------------------------------
func IsBitLit(bit uint32, bytes []byte) bool {
	which := uint32(bit / 8)
	index := uint32(BLOOM_WIDTH_IN_BYTES - which - 1)

	whence := uint32(bit % 8)
	mask := byte(1 << whence)

	return (bytes[index]&mask != 0)
}

func (bloom *BloomFilter) ReadBloomFilter(fileName string) (err error) {
	bloom.Range, err = cache.RangeFromFilename(fileName)
	if err != nil {
		return err
	}

	file, err := os.OpenFile(fileName, os.O_RDONLY, 0644)
	if err != nil {
		return err
	}

	err = binary.Read(file, binary.LittleEndian, &bloom.Count)
	if err != nil {
		return err
	}

	bloom.Blooms = make([]bloomBytes, bloom.Count)
	for i := uint32(0); i < bloom.Count; i++ {
		// fmt.Println("nBlooms:", bloom.Count)
		err = binary.Read(file, binary.LittleEndian, &bloom.Blooms[i].NInserted)
		if err != nil {
			return err
		}
		// fmt.Println("nInserted:", bloom.Blooms[i].NInserted)
		bloom.Blooms[i].Bytes = make([]byte, BLOOM_WIDTH_IN_BYTES)
		err = binary.Read(file, binary.LittleEndian, &bloom.Blooms[i].Bytes)
		if err != nil {
			return err
		}
	}

	return nil
}

func (bloom *BloomFilter) DisplayBloom(verbose int) {
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

func (bloom *BloomFilter) LightBits(bits [5]uint32) {
	if len(bloom.Blooms) == 0 {
		bloom.Blooms = append(bloom.Blooms, bloomBytes{})
		bloom.Blooms[0].Bytes = make([]byte, BLOOM_WIDTH_IN_BYTES)
	}

	loc := len(bloom.Blooms) - 1
	for _, bit := range bits {
		which := (bit / 8)
		whence := (bit % 8)
		index := BLOOM_WIDTH_IN_BYTES - which - 1
		mask := uint8(1 << whence)
		bloom.Blooms[loc].Bytes[index] |= mask
	}
	bloom.Blooms[loc].NInserted++

	if bloom.Blooms[loc].NInserted > MAX_ADDRS_IN_BLOOM {
		bloom.Blooms = append(bloom.Blooms, bloomBytes{})
	}
}
