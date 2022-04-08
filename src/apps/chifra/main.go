package main

/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd"

func main() {
	if cmd.Initialize() {
		cmd.Execute()
	}
}

// import (
// 	"fmt"
// 	"os"

// 	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
// 	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
// 	"github.com/ethereum/go-ethereum/common"
// )

// func main() {
// 	addr := common.HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b")
// 	fmt.Println(addr)
// 	for i := 0; i < 200; i++ {
// 		cachePath := cache.NewCachePath("mainnet", cache.Index_Bloom)
// 		bloomPath := cachePath.GetFullPath(cache.FileRange{First: 14435826, Last: 14438546}.String())
// 		if len(os.Args) > 1 {
// 			bloom, _ := NewChunk2(bloomPath)
// 			fmt.Printf("%d %t\n", i, IsMember2(bloom.Bloom, addr))
// 		} else {
// 			bloom, _ := index.NewChunk(bloomPath)
// 			fmt.Printf("%d %t\n", i, bloom.Bloom.IsMember(addr))
// 		}
// 	}
// 	fmt.Println()
// 	return
// }

// //----------------------------------------------------------------------
// func IsMember2(bloom index.ChunkBloom, addr common.Address) bool {
// 	for _, bb := range bloom.Blooms {
// 		if IsMember(bb, addr) {
// 			return true
// 		}
// 	}
// 	return false
// }

// //----------------------------------------------------------------------
// func IsMember(bb index.BloomBytes, addr common.Address) bool {
// 	whichBits := index.WhichBits(addr)
// 	for _, bit := range whichBits {
// 		if !index.IsBitLit(bit, bb.Bytes) {
// 			return false
// 		}
// 	}
// 	return true
// }

// func NewChunk2(path string) (chunk index.Chunk, err error) {
// 	bloomPath := index.ToBloomPath(path)
// 	chunk.Range, err = cache.RangeFromFilename(bloomPath)
// 	if err != nil {
// 		return
// 	}

// 	err = chunk.Bloom.Read(bloomPath)
// 	if err != nil {
// 		return
// 	}

// 	indexPath := index.ToIndexPath(path)
// 	chunk.Data, err = index.NewChunkData(indexPath)
// 	return
// }
