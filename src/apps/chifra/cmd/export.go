// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package cmd

// EXISTING_CODE
import (
	"os"

	exportPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/export"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// exportCmd represents the export command
var exportCmd = &cobra.Command{
	Use:     usageExport,
	Short:   shortExport,
	Long:    longExport,
	Version: versionText,
	RunE:    exportPkg.RunExport,
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
  - Articulating the export means turn the EVM's byte data into human-readable text (if possible).
  - For the --logs option, you may optionally specify one or more --emmitter, one or more --topics, or both.
  - The --logs option is significantly faster if you provide an --emitter or a --topic.
  - Neighbors include every address that appears in any transaction in which the export address also appears.`

func init() {
	exportCmd.Flags().SortFlags = false

	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Appearances, "appearances", "p", false, "export a list of appearances")
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Receipts, "receipts", "r", false, "export receipts instead of transactional data")
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Logs, "logs", "l", false, "export logs instead of transactional data")
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Traces, "traces", "t", false, "export traces instead of transactional data")
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Statements, "statements", "A", false, "export reconciliations instead of transactional data (requires --accounting option)")
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Neighbors, "neighbors", "n", false, "export the neighbors of the given address")
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Accounting, "accounting", "C", false, "attach accounting records to the exported data (applies to transactions export only)")
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Articulate, "articulate", "a", false, "articulate transactions, traces, logs, and outputs")
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Cache, "cache", "i", false, "write transactions to the cache (see notes)")
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().CacheTraces, "cache_traces", "R", false, "write traces to the cache (see notes)")
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Factory, "factory", "y", false, "scan for contract creations from the given address(es) and report address of those contracts")
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Count, "count", "U", false, "only available for --appearances mode, if present, return only the number of records")
	exportCmd.Flags().Uint64VarP(&exportPkg.GetOptions().FirstRecord, "first_record", "c", 0, "the first record to process")
	exportCmd.Flags().Uint64VarP(&exportPkg.GetOptions().MaxRecords, "max_records", "e", 250, "the maximum number of records to process before reporting")
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Relevant, "relevant", "", false, "for log and accounting export only, export only logs relevant to one of the given export addresses")
	exportCmd.Flags().StringSliceVarP(&exportPkg.GetOptions().Emitter, "emitter", "", nil, "for log export only, export only logs if emitted by one of these address(es)")
	exportCmd.Flags().StringSliceVarP(&exportPkg.GetOptions().Topic, "topic", "", nil, "for log export only, export only logs with this topic(s)")
	exportCmd.Flags().StringSliceVarP(&exportPkg.GetOptions().Asset, "asset", "", nil, "for the statements option only, export only reconciliations for this asset")
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Clean, "clean", "", false, "clean (i.e. remove duplicate appearances) from all existing monitors")
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Freshen, "freshen", "f", false, "freshen but do not print the exported data (hidden)")
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Staging, "staging", "s", false, "export transactions labeled staging (i.e. older than 28 blocks but not yet consolidated)")
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Unripe, "unripe", "u", false, "export transactions labeled upripe (i.e. less than 28 blocks old)")
	exportCmd.Flags().StringVarP(&exportPkg.GetOptions().Load, "load", "", "", "a comma separated list of dynamic traversers to load (hidden)")
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Reversed, "reversed", "", false, "produce results in reverse chronological order (hidden)")
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().ByDate, "by_date", "b", false, "produce results sorted by date (report by address otherwise) (hidden)")
	exportCmd.Flags().StringVarP(&exportPkg.GetOptions().SummarizeBy, "summarize_by", "z", "", `for --accounting only, summarize reconciliations by this time period (hidden)
One of [ yearly | quarterly | monthly | weekly | daily | hourly | blockly | tx ]`)
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Deep, "deep", "D", false, "for --neighbors option only, dig deeply into detail (otherwise, to and from only) (hidden)")
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().SkipDdos, "skip_ddos", "d", false, "toggle skipping over 2016 dDos transactions ('on' by default) (hidden)")
	exportCmd.Flags().Uint64VarP(&exportPkg.GetOptions().MaxTraces, "max_traces", "m", 250, "if --skip_ddos is on, this many traces defines what a ddos transaction is (hidden)")
	exportCmd.Flags().Uint64VarP(&exportPkg.GetOptions().FirstBlock, "first_block", "F", 0, "first block to process (inclusive)")
	exportCmd.Flags().Uint64VarP(&exportPkg.GetOptions().LastBlock, "last_block", "L", 0, "last block to process (inclusive)")
	if os.Getenv("TEST_MODE") != "true" {
		exportCmd.Flags().MarkHidden("freshen")
		exportCmd.Flags().MarkHidden("load")
		exportCmd.Flags().MarkHidden("reversed")
		exportCmd.Flags().MarkHidden("by_date")
		exportCmd.Flags().MarkHidden("summarize_by")
		exportCmd.Flags().MarkHidden("deep")
		exportCmd.Flags().MarkHidden("skip_ddos")
		exportCmd.Flags().MarkHidden("max_traces")
	}
	globals.InitGlobals(exportCmd, &exportPkg.GetOptions().Globals)

	exportCmd.SetUsageTemplate(UsageWithNotes(notesExport))
	exportCmd.SetOut(os.Stderr)

	chifraCmd.AddCommand(exportCmd)
}
