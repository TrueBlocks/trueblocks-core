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
/*
 * The file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */
package cmd

import (
	"os"

	exportPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/export"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/spf13/cobra"
)

// exportCmd represents the export command
var exportCmd = &cobra.Command{
	Use:   usageExport,
	Short: shortExport,
	Long:  longExport,
	Run:   exportPkg.Run,
	Args:  exportPkg.Validate,
}

var usageExport = `export [flags] <address> [address...] [topics...] [fourbytes...]

Arguments:
  addrs - one or more addresses (0x...) to export (required)
  topics - filter by one or more log topics (only for --logs option)
  fourbytes - filter by one or more fourbytes (only for transactions and trace options)`

var shortExport = "export full detail of transactions for one or more addresses"

var longExport = `Purpose:
  Export full detail of transactions for one or more addresses.`

var notesExport = `
Notes:
  - An address must start with '0x' and be forty-two characters long.
  - For the --logs option, you may optionally specify one or more --emmitter, one or more --topics, or both.
  - The --logs option is significantly faster if you provide an --emitter or a --topic.`

func init() {
	exportCmd.SetOut(os.Stderr)

	exportCmd.Flags().SortFlags = false
	exportCmd.PersistentFlags().SortFlags = false
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Appearances, "appearances", "p", false, "export a list of appearances")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Receipts, "receipts", "r", false, "export receipts instead of transaction list")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Statements, "statements", "A", false, "for use with --accounting option only, export only reconciliation statements")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Logs, "logs", "l", false, "export logs instead of transaction list")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Traces, "traces", "t", false, "export traces instead of transaction list")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Accounting, "accounting", "C", false, "export accounting records instead of transaction list")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Articulate, "articulate", "a", false, "articulate transactions, traces, logs, and outputs")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Cache, "cache", "i", false, "write transactions to the cache (see notes)")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Cache_Traces, "cache_traces", "R", false, "write traces to the cache (see notes)")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Factory, "factory", "y", false, "scan for contract creations from the given address(es) and report address of those contracts")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Count, "count", "U", false, "only available for --appearances mode, if present, return only the number of records")
	exportCmd.Flags().Uint64VarP(&exportPkg.Options.First_Record, "first_record", "c", 0, "the first record to process")
	exportCmd.Flags().Uint64VarP(&exportPkg.Options.Max_Records, "max_records", "e", 250, "the maximum number of records to process before reporting")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Relevant, "relevant", "", false, "for log and accounting export only, export only logs relevant to one of the given export addresses")
	exportCmd.Flags().StringSliceVarP(&exportPkg.Options.Emitter, "emitter", "", nil, "for log export only, export only logs if emitted by one of these address(es)")
	exportCmd.Flags().StringSliceVarP(&exportPkg.Options.Topic, "topic", "", nil, "for log export only, export only logs with this topic(s)")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Clean, "clean", "", false, "clean (i.e. remove duplicate appearances) from all existing monitors")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Freshen, "freshen", "f", false, "freshen but do not print the exported data (hidden)")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Staging, "staging", "s", false, "enable search of staging (not yet finalized) folder (hidden)")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Unripe, "unripe", "u", false, "enable search of unripe (neither staged nor finalized) folder (assumes --staging) (hidden)")
	exportCmd.Flags().StringVarP(&exportPkg.Options.Load, "load", "", "", "a comma separated list of dynamic traversers to load (hidden)")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Reversed, "reversed", "", false, "produce results in reverse chronological order (hidden)")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.By_Date, "by_date", "b", false, "produce results sorted by date (report by address otherwise) (hidden)")
	exportCmd.Flags().StringVarP(&exportPkg.Options.Summarize_By, "summarize_by", "z", "", `for --accounting only, summarize reconciliations by this time period (hidden)
One of [ yearly | quarterly | monthly | weekly | daily | hourly | blockly | tx ]`)
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Skip_Ddos, "skip_ddos", "d", false, "toggle skipping over 2016 dDos transactions ('on' by default) (hidden)")
	exportCmd.Flags().Uint64VarP(&exportPkg.Options.Max_Traces, "max_traces", "m", 250, "if --skip_ddos is on, this many traces defines what a ddos transaction is (hidden)")
	exportCmd.Flags().Uint64VarP(&exportPkg.Options.First_Block, "first_block", "F", 0, "first block to process (inclusive) (hidden)")
	exportCmd.Flags().Uint64VarP(&exportPkg.Options.Last_Block, "last_block", "L", 0, "last block to process (inclusive) (hidden)")
	if !utils.IsTestMode() {
		exportCmd.Flags().MarkHidden("freshen")
		exportCmd.Flags().MarkHidden("staging")
		exportCmd.Flags().MarkHidden("unripe")
		exportCmd.Flags().MarkHidden("load")
		exportCmd.Flags().MarkHidden("reversed")
		exportCmd.Flags().MarkHidden("by_date")
		exportCmd.Flags().MarkHidden("summarize_by")
		exportCmd.Flags().MarkHidden("skip_ddos")
		exportCmd.Flags().MarkHidden("max_traces")
		exportCmd.Flags().MarkHidden("first_block")
		exportCmd.Flags().MarkHidden("last_block")
	}
	exportCmd.Flags().SortFlags = false
	exportCmd.PersistentFlags().SortFlags = false

	exportCmd.SetUsageTemplate(UsageWithNotes(notesExport))
	rootCmd.AddCommand(exportCmd)
}
