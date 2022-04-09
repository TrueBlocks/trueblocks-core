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
// 	newVersion := len(os.Args) > 1
// 	// newVersion := true
// 	addr := common.HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b")
// 	for i := 0; i < 1000; i++ {
// 		i := 0
// 		cachePath := cache.NewCachePath("mainnet", cache.Index_Bloom)
// 		filePath := cachePath.GetFullPath(cache.FileRange{First: 14435826, Last: 14438546}.String())
// 		// fmt.Println(addr)
// 		// fmt.Println(cachePath)
// 		// fmt.Println(bloomPath)
// 		if newVersion {
// 			chunk, _ := index.NewChunk(filePath)
// 			// fmt.Println(chunk)
// 			fmt.Printf("%d %t\r", i, chunk.IsMember2(addr))
// 		} else {
// 			chunk, _ := index.NewChunk(filePath)
// 			err := index.ReadBloom(&chunk.Bloom, filePath)
// 			if err != nil {
// 				fmt.Println("Error", filePath, err)
// 				return
// 			}
// 			fmt.Printf("%d %t\r", i, chunk.Bloom.IsMember(addr))
// 		}
// 	}
// 	fmt.Println()
// 	return
// }
