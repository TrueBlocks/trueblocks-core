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
 * Parts of this file were generated with makeClass --gocmds. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
package cmd

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/utils"
	"github.com/spf13/cobra"
)

// exportCmd represents the export command
var exportCmd = &cobra.Command{
	Use:   usageExport,
	Short: shortExport,
	Long:  longExport,
	Run:   runExport,
	Args:  validateExportArgs,
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
  - An address must start with '0x' and be forty-two characters long.`

type exportOptionsType struct {
	appearances  bool
	receipts     bool
	statements   bool
	logs         bool
	traces       bool
	accounting   bool
	articulate   bool
	cache_txs    bool
	cache_traces bool
	factory      bool
	emitter      bool
	source       []string
	relevant     bool
	count        bool
	first_record uint64
	max_records  uint64
	clean        bool
	freshen      bool
	staging      bool
	unripe       bool
	load         string
	reversed     bool
	by_date      bool
	summarize_by string
	skip_ddos    bool
	max_traces   uint64
	first_block  uint64
	last_block   uint64
}

var ExportOpts exportOptionsType

func init() {
	exportCmd.SetOut(os.Stderr)

	exportCmd.Flags().SortFlags = false
	exportCmd.PersistentFlags().SortFlags = false
	exportCmd.Flags().BoolVarP(&ExportOpts.appearances, "appearances", "p", false, "export a list of appearances")
	exportCmd.Flags().BoolVarP(&ExportOpts.receipts, "receipts", "r", false, "export receipts instead of transaction list")
	exportCmd.Flags().BoolVarP(&ExportOpts.statements, "statements", "A", false, "for use with --accounting option only, export only reconciliation statements")
	exportCmd.Flags().BoolVarP(&ExportOpts.logs, "logs", "l", false, "export logs instead of transaction list")
	exportCmd.Flags().BoolVarP(&ExportOpts.traces, "traces", "t", false, "export traces instead of transaction list")
	exportCmd.Flags().BoolVarP(&ExportOpts.accounting, "accounting", "C", false, "export accounting records instead of transaction list")
	exportCmd.Flags().BoolVarP(&ExportOpts.articulate, "articulate", "a", false, "articulate transactions, traces, logs, and outputs")
	exportCmd.Flags().BoolVarP(&ExportOpts.cache_txs, "cache_txs", "i", false, "write transactions to the cache (see notes)")
	exportCmd.Flags().BoolVarP(&ExportOpts.cache_traces, "cache_traces", "R", false, "write traces to the cache (see notes)")
	exportCmd.Flags().BoolVarP(&ExportOpts.factory, "factory", "y", false, "scan for contract creations from the given address(es) and report address of those contracts")
	exportCmd.Flags().BoolVarP(&ExportOpts.emitter, "emitter", "", false, "for log export only, export only if one of the given export addresses emitted the event")
	exportCmd.Flags().StringSliceVarP(&ExportOpts.source, "source", "", nil, "for log export only, export only one of these addresses emitted the event")
	exportCmd.Flags().BoolVarP(&ExportOpts.relevant, "relevant", "", false, "for log and accounting export only, if true export only logs relevant to one of the given export addresses")
	exportCmd.Flags().BoolVarP(&ExportOpts.count, "count", "U", false, "only available for --appearances mode, if present, return only the number of records")
	exportCmd.Flags().Uint64VarP(&ExportOpts.first_record, "first_record", "c", 0, "the first record to process")
	exportCmd.Flags().Uint64VarP(&ExportOpts.max_records, "max_records", "e", 250, "the maximum number of records to process before reporting")
	exportCmd.Flags().BoolVarP(&ExportOpts.clean, "clean", "", false, "clean (i.e. remove duplicate appearances) from all existing monitors")
	exportCmd.Flags().BoolVarP(&ExportOpts.freshen, "freshen", "f", false, "freshen but do not print the exported data (hidden)")
	exportCmd.Flags().BoolVarP(&ExportOpts.staging, "staging", "s", false, "enable search of staging (not yet finalized) folder (hidden)")
	exportCmd.Flags().BoolVarP(&ExportOpts.unripe, "unripe", "u", false, "enable search of unripe (neither staged nor finalized) folder (assumes --staging) (hidden)")
	exportCmd.Flags().StringVarP(&ExportOpts.load, "load", "", "", "a comma separated list of dynamic traversers to load (hidden)")
	exportCmd.Flags().BoolVarP(&ExportOpts.reversed, "reversed", "", false, "produce results in reverse chronological order (hidden)")
	exportCmd.Flags().BoolVarP(&ExportOpts.by_date, "by_date", "b", false, "produce results sorted by date (report by address otherwise) (hidden)")
	exportCmd.Flags().StringVarP(&ExportOpts.summarize_by, "summarize_by", "z", "", `for --accounting only, summarize reconciliations by this time period (hidden)
One of [ yearly | quarterly | monthly | weekly | daily | hourly | blockly | tx ]`)
	exportCmd.Flags().BoolVarP(&ExportOpts.skip_ddos, "skip_ddos", "d", false, "toggle skipping over 2016 dDos transactions ('on' by default) (hidden)")
	exportCmd.Flags().Uint64VarP(&ExportOpts.max_traces, "max_traces", "m", 250, "if --skip_ddos is on, this many traces defines what a ddos transaction is (hidden)")
	exportCmd.Flags().Uint64VarP(&ExportOpts.first_block, "first_block", "F", 0, "first block to process (inclusive) (hidden)")
	exportCmd.Flags().Uint64VarP(&ExportOpts.last_block, "last_block", "L", 0, "last block to process (inclusive) (hidden)")
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

func TestLogExport(args []string) {
	if !utils.IsTestMode() {
		return
	}
}

// EXISTING_CODE
// EXISTING_CODE
