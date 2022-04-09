package index

// import (
// 	"encoding/binary"
// 	"fmt"
// 	"io"

// 	"github.com/ethereum/go-ethereum/common"
// )

// func (chunk *Chunk) IsMember2(addr common.Address) bool {
// 	for i := 0; i < int(chunk.Bloom.Count); i++ {
// 		if chunk.Bloom.IsMember2(i, addr) {
// 			return true
// 		}
// 	}
// 	return false
// }

// func (bloom *ChunkBloom) IsMember2(which int, addr common.Address) bool {
// 	const ARRAY_START = 4
// 	const ITEM_SIZE_IN_BYTES = 4 + BLOOM_WIDTH_IN_BYTES

// 	var positions []int
// 	for i := 0; i < int(bloom.Count); i++ {
// 		positions = append(positions, ARRAY_START+(i*ITEM_SIZE_IN_BYTES))
// 	}
// 	for i, p := range positions {
// 		fmt.Println(i, p)
// 	}
// 	fmt.Println(bloom.Range, bloom.Size)

// 	whichBits := WhichBits(addr)
// 	for i, p := range positions {
// 		hitCount := 0
// 		for j, bit := range whichBits {
// 			byt := bit / 8
// 			which := bit % 8
// 			mask := uint8(1 << which)
// 			// fByte := float64(bit) / float64(8)
// 			readLoc := int64(p + int(byt))
// 			bloom.File.Seek(readLoc, io.SeekStart)
// 			var val uint8
// 			err := binary.Read(bloom.File, binary.LittleEndian, &val)
// 			if err != nil {
// 				return false
// 			}
// 			fmt.Printf("readingAt: %d %d % 9d % 9d % 9d % 9d % 9d % 9d %d %d\n", i, j, p, bit, which, mask, byt, readLoc, val, val&mask)
// 			if val&mask > 0 {
// 				hitCount++
// 			}
// 		}
// 		fmt.Println("hitCount:", hitCount)
// 		if hitCount == len(whichBits) {
// 			return true
// 		}
// 	}
// 	fmt.Println()
// 	return true
// }
