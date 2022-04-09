package index

import (
	"encoding/binary"
	"fmt"
	"io"

	"github.com/ethereum/go-ethereum/common"
)

func (bloom *ChunkBloom) IsMember_Old(addr common.Address) bool {
	for _, bb := range bloom.Blooms {
		if bloom.IsMember_Old_Inner(&bb, addr) {
			return true
		}
	}
	return false
}

func (bloom *ChunkBloom) IsMember_New(addr common.Address) bool {
	const COUNT_SIZE = uint32(4)
	const NINSERT_SIZE = uint32(4)
	offset := COUNT_SIZE
	for j := 0; j < int(bloom.Count); j++ {
		offset += NINSERT_SIZE
		if bloom.IsMember_New_Inner(offset, addr) {
			return true
		}
		offset += BLOOM_WIDTH_IN_BYTES
	}
	return false
}

type BitTester struct {
	offset uint32
	bit    uint32
	bytes  *BloomBytes
}

func (bloom *ChunkBloom) IsMember_Old_Inner(bb *BloomBytes, addr common.Address) bool {
	var tester = BitTester{bytes: bb}
	whichBits := WhichBits(addr)
	for _, bit := range whichBits {
		tester.bit = bit
		if !bloom.isBitLitMemory(&tester, bb.Bytes) {
			return false
		}
	}
	return true
}

func (bloom *ChunkBloom) IsMember_New_Inner(offset uint32, addr common.Address) bool {
	var tester = BitTester{offset: offset}
	whichBits := WhichBits(addr)
	for _, bit := range whichBits {
		tester.bit = bit
		if !bloom.isBitLitDisc(&tester, nil) {
			return false
		}
	}
	return true
}

// IsBitLit returns true if the given bit is lit in the given byte array
func (bloom *ChunkBloom) isBitLitMemory(tester *BitTester, bytes []byte) bool {
	which := uint32(tester.bit / 8)
	index := uint32(BLOOM_WIDTH_IN_BYTES - which - 1)

	whence := uint32(tester.bit % 8)
	mask := byte(1 << whence)

	var byt uint8
	byt = bytes[index]
	res := byt & mask

	// fmt.Fprintf(os.Stdout, "%d-%d-%d: % 9d\t% 9d\t% 9d\t% 9d\t% 9d\t% 9d\t%t\n", i, j, k, which, index, whence, mask, byt, res, (res != 0))
	return (res != 0)
}

func (bloom *ChunkBloom) isBitLitDisc(tester *BitTester, bytes []byte) bool {
	which := uint32(tester.bit / 8)
	index := uint32(BLOOM_WIDTH_IN_BYTES - which - 1)

	whence := uint32(tester.bit % 8)
	mask := byte(1 << whence)

	var byt uint8
	_, err := bloom.File.Seek(int64(tester.offset+index), io.SeekStart)
	if err != nil {
		fmt.Println("Seek error:", err)
		return false
	}

	err = binary.Read(bloom.File, binary.LittleEndian, &byt)
	if err != nil {
		fmt.Println("Read error:", err)
		return false
	}

	res := byt & mask
	// fmt.Fprintf(os.Stdout, "%d-%d-%d: % 9d\t% 9d\t% 9d\t% 9d\t% 9d\t% 9d\t%t\n", i, j, k, which, index, whence, mask, byt, res, (res != 0))
	return (res != 0)
}
