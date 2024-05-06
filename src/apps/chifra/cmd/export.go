// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package cmd

// EXISTING_CODE
import (
	"os"

	exportPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/export"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// exportCmd represents the export command
var exportCmd = &cobra.Command{
	Use:     usageExport,
	Long:    longExport,
	Version: versionText,
	PreRun: outputHelpers.PreRunWithJsonWriter("export", func() *globals.GlobalOptions {
		return &exportPkg.GetOptions().Globals
	}),
	RunE: file.RunWithFileSupport("export", exportPkg.RunExport, exportPkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &exportPkg.GetOptions().Globals
	}),
}

const usageExport = `export [flags] <address> [address...] [topics...] [fourbytes...]

Arguments:
  addrs - one or more addresses (0x...) to export (required)
  topics - filter by one or more log topics (only for --logs option)
  fourbytes - filter by one or more fourbytes (only for transactions and trace options)`

const longExport = `Purpose:
  Export full details of transactions for one or more addresses.`

const notesExport = `
Notes:
  - An address must be either an ENS name or start with '0x' and be forty-two characters long.
  - Articulating the export means turn the EVM's byte data into human-readable text (if possible).
  - For the --logs option, you may optionally specify one or more --emitter, one or more --topics, or both.
  - The --logs option is significantly faster if you provide an --emitter or a --topic.
  - Neighbors include every address that appears in any transaction in which the export address also appears.
  - If present, --first_/--last_block are applied, followed by user-supplied filters such as asset or topic, followed by --first_/--max_record if present.
  - The --first_record and --max_record options are zero-based (as are the block options).
  - The _block and _record filters are ignored when used with the --count option.
  - If the --reversed option is present, the appearance list is reversed prior to all processing (including filtering).
  - The --decache option will remove all cache items (blocks, transactions, traces, etc.) for the given address(es).
  - The --withdrawals option is only available on certain chains. It is ignored otherwise.`

func init() {
	var capabilities caps.Capability // capabilities for chifra export
	capabilities = capabilities.Add(caps.Default)
	capabilities = capabilities.Add(caps.Caching)
	capabilities = capabilities.Add(caps.Ether)

	exportCmd.Flags().SortFlags = false

	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Appearances, "appearances", "p", false, `export a list of appearances`)
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Receipts, "receipts", "r", false, `export receipts instead of transactional data`)
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Logs, "logs", "l", false, `export logs instead of transactional data`)
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Traces, "traces", "t", false, `export traces instead of transactional data`)
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Neighbors, "neighbors", "n", false, `export the neighbors of the given address`)
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Accounting, "accounting", "C", false, `attach accounting records to the exported data (applies to transactions export only)`)
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Statements, "statements", "A", false, `for the accounting options only, export only statements`)
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Balances, "balances", "b", false, `traverse the transaction history and show each change in ETH balances`)
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Withdrawals, "withdrawals", "i", false, `export withdrawals for the given address`)
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Articulate, "articulate", "a", false, `articulate transactions, traces, logs, and outputs`)
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().CacheTraces, "cache_traces", "R", false, `force the transaction's traces into the cache`)
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Count, "count", "U", false, `for --appearances mode only, display only the count of records`)
	exportCmd.Flags().Uint64VarP(&exportPkg.GetOptions().FirstRecord, "first_record", "c", 0, `the first record to process`)
	exportCmd.Flags().Uint64VarP(&exportPkg.GetOptions().MaxRecords, "max_records", "e", 250, `the maximum number of records to process`)
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Relevant, "relevant", "N", false, `for log and accounting export only, export only logs relevant to one of the given export addresses`)
	exportCmd.Flags().StringSliceVarP(&exportPkg.GetOptions().Emitter, "emitter", "m", nil, `for the --logs option only, filter logs to show only those logs emitted by the given address(es)`)
	exportCmd.Flags().StringSliceVarP(&exportPkg.GetOptions().Topic, "topic", "B", nil, `for the --logs option only, filter logs to show only those with this topic(s)`)
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Reverted, "reverted", "V", false, `export only transactions that were reverted`)
	exportCmd.Flags().StringSliceVarP(&exportPkg.GetOptions().Asset, "asset", "P", nil, `for the accounting options only, export statements only for this asset`)
	exportCmd.Flags().StringVarP(&exportPkg.GetOptions().Flow, "flow", "f", "", `for the accounting options only, export statements with incoming, outgoing, or zero value
One of [ in | out | zero ]`)
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Factory, "factory", "y", false, `for --traces only, report addresses created by (or self-destructed by) the given address(es)`)
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Unripe, "unripe", "u", false, `export transactions labeled upripe (i.e. less than 28 blocks old)`)
	exportCmd.Flags().StringVarP(&exportPkg.GetOptions().Load, "load", "O", "", `a comma separated list of dynamic traversers to load (hidden)`)
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().Reversed, "reversed", "E", false, `produce results in reverse chronological order`)
	exportCmd.Flags().BoolVarP(&exportPkg.GetOptions().NoZero, "no_zero", "z", false, `for the --count option only, suppress the display of zero appearance accounts`)
	exportCmd.Flags().Uint64VarP((*uint64)(&exportPkg.GetOptions().FirstBlock), "first_block", "F", 0, `first block to process (inclusive)`)
	exportCmd.Flags().Uint64VarP((*uint64)(&exportPkg.GetOptions().LastBlock), "last_block", "L", 0, `last block to process (inclusive)`)
	if os.Getenv("TEST_MODE") != "true" {
		_ = exportCmd.Flags().MarkHidden("load")
	}
	globals.InitGlobals("export", exportCmd, &exportPkg.GetOptions().Globals, capabilities)

	exportCmd.SetUsageTemplate(UsageWithNotes(notesExport))
	exportCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// This no-op makes scripting a bit easier. You may provide `--txs` option to the chifra export command, and it will be the same as no parameters at all.
	var unused bool
	exportCmd.Flags().BoolVarP(&unused, "txs", "", false, "no-op options shows transactions (same as default)")
	_ = exportCmd.Flags().MarkHidden("txs")
	// EXISTING_CODE

	chifraCmd.AddCommand(exportCmd)
}
