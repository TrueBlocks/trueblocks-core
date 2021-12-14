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
	Version: "GHC-TrueBlocks//0.18.0-alpha",
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

	exportCmd.Flags().BoolVarP(&exportPkg.Options.Appearances, "appearances", "p", false, "export a list of appearances")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Receipts, "receipts", "r", false, "export receipts instead of transactional data")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Logs, "logs", "l", false, "export logs instead of transactional data")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Traces, "traces", "t", false, "export traces instead of transactional data")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Statements, "statements", "A", false, "export reconciliations instead of transactional data (requires --accounting option)")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Neighbors, "neighbors", "n", false, "export the neighbors of the given address")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Accounting, "accounting", "C", false, "attach accounting records to the exported data (applies to transactions export only)")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Articulate, "articulate", "a", false, "articulate transactions, traces, logs, and outputs")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Cache, "cache", "i", false, "write transactions to the cache (see notes)")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.CacheTraces, "cache_traces", "R", false, "write traces to the cache (see notes)")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Factory, "factory", "y", false, "scan for contract creations from the given address(es) and report address of those contracts")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Count, "count", "U", false, "only available for --appearances mode, if present, return only the number of records")
	exportCmd.Flags().Uint64VarP(&exportPkg.Options.FirstRecord, "first_record", "c", 0, "the first record to process")
	exportCmd.Flags().Uint64VarP(&exportPkg.Options.MaxRecords, "max_records", "e", 250, "the maximum number of records to process before reporting")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Relevant, "relevant", "", false, "for log and accounting export only, export only logs relevant to one of the given export addresses")
	exportCmd.Flags().StringSliceVarP(&exportPkg.Options.Emitter, "emitter", "", nil, "for log export only, export only logs if emitted by one of these address(es)")
	exportCmd.Flags().StringSliceVarP(&exportPkg.Options.Topic, "topic", "", nil, "for log export only, export only logs with this topic(s)")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Clean, "clean", "", false, "clean (i.e. remove duplicate appearances) from all existing monitors")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Freshen, "freshen", "f", false, "freshen but do not print the exported data (hidden)")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Staging, "staging", "s", false, "enable search of staging (not yet finalized) folder (hidden)")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Unripe, "unripe", "u", false, "enable search of unripe (neither staged nor finalized) folder (assumes --staging) (hidden)")
	exportCmd.Flags().StringVarP(&exportPkg.Options.Load, "load", "", "", "a comma separated list of dynamic traversers to load (hidden)")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Reversed, "reversed", "", false, "produce results in reverse chronological order (hidden)")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.ByDate, "by_date", "b", false, "produce results sorted by date (report by address otherwise) (hidden)")
	exportCmd.Flags().StringVarP(&exportPkg.Options.SummarizeBy, "summarize_by", "z", "", `for --accounting only, summarize reconciliations by this time period (hidden)
One of [ yearly | quarterly | monthly | weekly | daily | hourly | blockly | tx ]`)
	exportCmd.Flags().BoolVarP(&exportPkg.Options.Deep, "deep", "D", false, "for --neighbors option only, dig deeply into detail (otherwise, to and from only) (hidden)")
	exportCmd.Flags().BoolVarP(&exportPkg.Options.SkipDdos, "skip_ddos", "d", false, "toggle skipping over 2016 dDos transactions ('on' by default) (hidden)")
	exportCmd.Flags().Uint64VarP(&exportPkg.Options.MaxTraces, "max_traces", "m", 250, "if --skip_ddos is on, this many traces defines what a ddos transaction is (hidden)")
	exportCmd.Flags().Uint64VarP(&exportPkg.Options.FirstBlock, "first_block", "F", 0, "first block to process (inclusive) (hidden)")
	exportCmd.Flags().Uint64VarP(&exportPkg.Options.LastBlock, "last_block", "L", 0, "last block to process (inclusive) (hidden)")
	if os.Getenv("TEST_MODE") != "true" {
		exportCmd.Flags().MarkHidden("freshen")
		exportCmd.Flags().MarkHidden("staging")
		exportCmd.Flags().MarkHidden("unripe")
		exportCmd.Flags().MarkHidden("load")
		exportCmd.Flags().MarkHidden("reversed")
		exportCmd.Flags().MarkHidden("by_date")
		exportCmd.Flags().MarkHidden("summarize_by")
		exportCmd.Flags().MarkHidden("deep")
		exportCmd.Flags().MarkHidden("skip_ddos")
		exportCmd.Flags().MarkHidden("max_traces")
		exportCmd.Flags().MarkHidden("first_block")
		exportCmd.Flags().MarkHidden("last_block")
	}
	globals.InitGlobals(exportCmd, &exportPkg.Options.Globals)

	exportCmd.SetUsageTemplate(UsageWithNotes(notesExport))
	exportCmd.SetOut(os.Stderr)

	chifraCmd.AddCommand(exportCmd)
}
