package export

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

func Run(cmd *cobra.Command, args []string) error {
	options := ""
	if Options.Appearances {
		options += " --appearances"
	}
	if Options.Receipts {
		options += " --receipts"
	}
	if Options.Statements {
		options += " --statements"
	}
	if Options.Logs {
		options += " --logs"
	}
	if Options.Traces {
		options += " --traces"
	}
	if Options.Accounting {
		options += " --accounting"
	}
	if Options.Articulate {
		options += " --articulate"
	}
	if Options.Cache {
		options += " --cache"
	}
	if Options.CacheTraces {
		options += " --cache_traces"
	}
	if Options.Neighbors {
		options += " --neighbors"
	}
	if Options.Factory {
		options += " --factory"
	}
	for _, e := range Options.Emitter {
		options += " --emitter " + e
	}
	for _, t := range Options.Topic {
		options += " --topic " + t
	}
	if Options.Relevant {
		options += " --relevant"
	}
	if Options.Count {
		options += " --count"
	}
	if Options.FirstRecord > 0 {
		options += " --first_record " + fmt.Sprintf("%d", Options.FirstRecord)
	}
	if Options.MaxRecords > 0 && Options.MaxRecords != 250 {
		options += " --max_records " + fmt.Sprintf("%d", Options.MaxRecords)
	}
	if Options.Clean {
		options += " --clean"
	}
	if Options.Freshen {
		options += " --freshen"
	}
	if Options.Staging {
		options += " --staging"
	}
	if Options.Unripe {
		options += " --unripe"
	}
	if len(Options.Load) > 0 {
		options += " --load " + Options.Load
	}
	if Options.Reversed {
		options += " --reversed"
	}
	if Options.ByDate {
		options += " --by_date"
	}
	if len(Options.SummarizeBy) > 0 {
		options += " --summarize_by " + Options.SummarizeBy
	}
	if Options.SkipDdos {
		options += " --skip_ddos"
	}
	if Options.MaxTraces != 250 {
		options += " --max_traces " + fmt.Sprintf("%d", Options.MaxTraces)
	}
	if Options.FirstBlock > 0 {
		options += " --first_block " + fmt.Sprintf("%d", Options.FirstBlock)
	}
	if Options.LastBlock > 0 {
		options += " --last_block " + fmt.Sprintf("%d", Options.LastBlock)
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}

	return root.PassItOn("acctExport", &Options.Globals, options, arguments)
}
