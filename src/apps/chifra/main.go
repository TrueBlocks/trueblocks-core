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
// 	"strings"

// 	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
// 	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
// 	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
// 	"github.com/ethereum/go-ethereum/common"
// )

// func main() {
// 	contents := utils.AsciiFileToString("./addresses.csv")
// 	lines := strings.Split(contents, "\n")
// 	for _, line := range lines {
// 		if line == "0x19f494583c7c933be7b0ee58104ddafac1e8adfa" {
// 			fmt.Println()
// 		}
// 		addr := common.HexToAddress(line)
// 		cachePath := cache.NewCachePath("mainnet", cache.Index_Bloom)
// 		filePath := cachePath.GetFullPath(cache.FileRange{First: 14435826, Last: 14438546}.String())
// 		which := len(os.Args) > 1 && os.Args[1] == "old"
// 		for i := 0; i < 1; i++ {
// 			if which {
// 				oldChunk, _ := index.NewChunk(filePath)
// 				index.ReadBloom(&oldChunk.Bloom, filePath)
// 				fmt.Fprintf(os.Stdout, "=========== %d %s ==========\n", i, line)
// 				fmt.Fprintf(os.Stdout, "%d: %t\n", i, oldChunk.Bloom.IsMember_Old(i, addr))
// 				oldChunk.Close()
// 			} else {
// 				newChunk, _ := index.NewChunk(filePath)
// 				fmt.Fprintf(os.Stdout, "=========== %d %s ==========\n", i, line)
// 				fmt.Fprintf(os.Stdout, "%d: %t\n", i, newChunk.Bloom.IsMember_New(i, addr))
// 				newChunk.Close()
// 			}
// 		}
// 	}

// 	return
// }
