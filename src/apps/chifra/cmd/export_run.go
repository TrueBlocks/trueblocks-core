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
package cmd

import (
	"fmt"

	"github.com/spf13/cobra"
)

func validateExportArgs(cmd *cobra.Command, args []string) error {
	err := validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}

	return nil
}

func runExport(cmd *cobra.Command, args []string) {
	options := ""
	if ExportOpts.appearances {
		options += " --appearances"
	}
	if ExportOpts.receipts {
		options += " --receipts"
	}
	if ExportOpts.statements {
		options += " --statements"
	}
	if ExportOpts.logs {
		options += " --logs"
	}
	if ExportOpts.traces {
		options += " --traces"
	}
	if ExportOpts.accounting {
		options += " --accounting"
	}
	if ExportOpts.articulate {
		options += " --articulate"
	}
	if ExportOpts.cache_txs {
		options += " --cache_txs"
	}
	if ExportOpts.cache_traces {
		options += " --cache_traces"
	}
	if ExportOpts.factory {
		options += " --factory"
	}
	if ExportOpts.emitter {
		options += " --emitter"
	}
	for _, t := range ExportOpts.source {
		options += " --source " + t
	}
	if ExportOpts.relevant {
		options += " --relevant"
	}
	if ExportOpts.count {
		options += " --count"
	}
	if ExportOpts.first_record > 0 {
		options += " --first_record " + fmt.Sprintf("%d", ExportOpts.first_record)
	}
	if ExportOpts.max_records > 0 && ExportOpts.max_records != 250 {
		options += " --max_records " + fmt.Sprintf("%d", ExportOpts.max_records)
	}
	if ExportOpts.clean {
		options += " --clean"
	}
	if ExportOpts.freshen {
		options += " --freshen"
	}
	if ExportOpts.staging {
		options += " --staging"
	}
	if ExportOpts.unripe {
		options += " --unripe"
	}
	if len(ExportOpts.load) > 0 {
		options += " --load " + ExportOpts.load
	}
	if ExportOpts.reversed {
		options += " --reversed"
	}
	if ExportOpts.by_date {
		options += " --by_date"
	}
	if len(ExportOpts.summarize_by) > 0 {
		options += " --summarize_by " + ExportOpts.summarize_by
	}
	if ExportOpts.skip_ddos {
		options += " --skip_ddos"
	}
	if ExportOpts.max_traces != 250 {
		options += " --max_traces " + fmt.Sprintf("%d", ExportOpts.max_traces)
	}
	if ExportOpts.first_block > 0 {
		options += " --first_block " + fmt.Sprintf("%d", ExportOpts.first_block)
	}
	if ExportOpts.last_block > 0 {
		options += " --last_block " + fmt.Sprintf("%d", ExportOpts.last_block)
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn(GetCommandPath("acctExport"), options, arguments)
}
