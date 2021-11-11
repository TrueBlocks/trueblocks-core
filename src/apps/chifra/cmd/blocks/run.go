package blocks

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

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/root"
	"github.com/spf13/cobra"
)

func Run(cmd *cobra.Command, args []string) {
	options := ""
	if Options.Hashes {
		options += " --hashes"
	}
	if Options.Uncles {
		options += " --uncles"
	}
	if Options.Trace {
		options += " --trace"
	}
	if Options.Apps {
		options += " --apps"
	}
	if Options.Uniq {
		options += " --uniq"
	}
	if Options.Uniq_Tx {
		options += " --uniq_tx"
	}
	if Options.Logs {
		options += " --logs"
	}
	for _, e := range Options.Emitter {
		options += " --emitter " + e
	}
	for _, t := range Options.Topic {
		options += " --topic " + t
	}
	if Options.Articulate {
		options += " --articulate"
	}
	if Options.Big_Range != 500 {
		options += " --big_range " + fmt.Sprintf("%d", Options.Big_Range)
	}
	if Options.Count {
		options += " --count"
	}
	if Options.Cache {
		options += " --cache"
	}
	if Options.List > 0 {
		options += " --list " + fmt.Sprintf("%d", Options.List)
	}
	if Options.List_Count != 20 {
		options += " --list_count " + fmt.Sprintf("%d", Options.List_Count)
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	root.PassItOn("getBlocks", options, arguments)
}
