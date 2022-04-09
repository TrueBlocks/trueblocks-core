package index

import (
	"encoding/binary"
	"fmt"
	"io"

	"github.com/ethereum/go-ethereum/common"
)

func (bloom *ChunkBloom) IsMember_Old(addr common.Address) bool {
	whichBits := WhichBits(addr)
	for _, bb := range bloom.Blooms {
		var tester = bitChecker{bytes: bb.Bytes, whichBits: whichBits}
		if bloom.isMember(&tester) {
			return true
		}
	}
	return false
}

func (bloom *ChunkBloom) IsMember_New(addr common.Address) bool {
	whichBits := WhichBits(addr)
	offset := uint32(4)
	for j := 0; j < int(bloom.Count); j++ {
		offset += uint32(4)
		var tester = bitChecker{offset: offset, whichBits: whichBits}
		if bloom.isMember(&tester) {
			return true
		}
		offset += BLOOM_WIDTH_IN_BYTES
	}
	return false
}

func (bloom *ChunkBloom) isMember(tester *bitChecker) bool {
	for _, bit := range tester.whichBits {
		tester.bit = bit
		if !bloom.isBitLit(tester) {
			return false
		}
	}
	return true
}

type bitChecker struct {
	whichBits [5]uint32
	offset    uint32
	bit       uint32
	bytes     []byte
}

// isBitLit returns true if the given bit is lit in the given byte array
func (bloom *ChunkBloom) isBitLit(tester *bitChecker) bool {
	which := uint32(tester.bit / 8)
	index := uint32(BLOOM_WIDTH_IN_BYTES - which - 1)

	whence := uint32(tester.bit % 8)
	mask := byte(1 << whence)

	var res uint8
	if tester.bytes != nil {
		// In some cases, we've already read the bytes into memory, so use them if they're here
		byt := tester.bytes[index]
		res = byt & mask

	} else {
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

		res = byt & mask
	}

	// fmt.Fprintf(os.Stdout, "%d-%d-%d: % 9d\t% 9d\t% 9d\t% 9d\t% 9d\t% 9d\t%t\n", i, j, k, which, index, whence, mask, byt, res, (res != 0))
	return (res != 0)
}
