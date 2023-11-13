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

package main

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/query"
)

func main() {
	defer Cleanup()
	// pprofFile, pprofErr := os.OpenFile("cpu.pprof", O_RDWR|O_CREATE|O_TRUNC, 0666)
	// if pprofErr != nil {
	// 	logger.Fatal(pprofErr)
	// }
	// pprof.StartCPUProfile(pprofFile)
	// defer pprof.StopCPUProfile()

	if cmd.Initialize() {
		cmd.Execute()
	}
}

// Cleanup gets called before main exits.
func Cleanup() {
	query.CloseDebugger()
}
