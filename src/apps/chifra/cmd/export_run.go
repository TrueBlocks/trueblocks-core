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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/spf13/cobra"
)

func validateExportArgs(cmd *cobra.Command, args []string) error {
	err := validate.ValidateOneAddr(args)
	if err != nil {
		return err
	}

	err = validate.ValidateEnum("--summarize_by", ExportOpts.summarize_by, "[yearly|quarterly|monthly|weekly|daily|hourly|blockly|tx]")
	if err != nil {
		return err
	}

	if len(ExportOpts.summarize_by) > 0 && !ExportOpts.accounting {
		return validate.Usage("You may use --summarized_by only with the --accounting option.")
	}

	err = validateGlobalFlags(cmd, args)
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
	if ExportOpts.cache {
		options += " --cache"
	}
	if ExportOpts.cache_traces {
		options += " --cache_traces"
	}
	if ExportOpts.factory {
		options += " --factory"
	}
	for _, e := range ExportOpts.emitter {
		options += " --emitter " + e
	}
	for _, t := range ExportOpts.topic {
		options += " --topic " + t
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
	PassItOn("acctExport", options, arguments)
}
