package index

// import (
// 	"encoding/binary"
// 	"fmt"
// 	"io"
// 	"os"

// 	"github.com/ethereum/go-ethereum/common"
// )

// func (bloom *ChunkBloom) IsMember_New(i int, addr common.Address) bool {
// 	fmt.Println("Count:", bloom.Count)
// 	const COUNT_SIZE = 4
// 	const NINSERT_SIZE = 4
// 	offset := COUNT_SIZE
// 	for j := 0; j < int(bloom.Count); j++ {
// 		offset += NINSERT_SIZE
// 		fmt.Printf("--------- bloom: %d-%d -----------\n", i, j)
// 		if bloom.IsMember_New_Inner(i, j, offset, addr) {
// 			return true
// 		}
// 		offset += BLOOM_WIDTH_IN_BYTES
// 	}
// 	return false
// }

// func (bloom *ChunkBloom) IsMember_New_Inner(i, j, offset int, addr common.Address) bool {
// 	whichBits := WhichBits(addr)
// 	for k, bit := range whichBits {
// 		if !bloom.IsBitLit_New(i, j, k, offset, bit) {
// 			return false
// 		}
// 	}
// 	return true
// }

// func (bloom *ChunkBloom) IsBitLit_New(i, j, k, offset int, bit uint32) bool {
// 	which := uint32(bit / 8)
// 	index := uint32(BLOOM_WIDTH_IN_BYTES - which - 1)

// 	whence := uint32(bit % 8)
// 	mask := byte(1 << whence)

// 	var byt uint8
// 	_, err := bloom.File.Seek(int64(offset+int(index)), io.SeekStart)
// 	if err != nil {
// 		fmt.Println("Seek error:", err)
// 		return false
// 	}

// 	err = binary.Read(bloom.File, binary.LittleEndian, &byt)
// 	if err != nil {
// 		fmt.Println("Read error:", err)
// 		return false
// 	}

// 	res := byt & mask
// 	fmt.Fprintf(os.Stdout, "%d-%d-%d: % 9d\t% 9d\t% 9d\t% 9d\t% 9d\t% 9d\t%t\n", i, j, k, which, index, whence, mask, byt, res, (res != 0))
// 	return (res != 0)
// }
