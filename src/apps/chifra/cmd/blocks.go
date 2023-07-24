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

	blocksPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/blocks"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// blocksCmd represents the blocks command
var blocksCmd = &cobra.Command{
	Use:     usageBlocks,
	Short:   shortBlocks,
	Long:    longBlocks,
	Version: versionText,
	PreRun: outputHelpers.PreRunWithJsonWriter("blocks", func() *globals.GlobalOptions {
		return &blocksPkg.GetOptions().Globals
	}),
	RunE: file.RunWithFileSupport("blocks", blocksPkg.RunBlocks, blocksPkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &blocksPkg.GetOptions().Globals
	}),
}

const usageBlocks = `blocks [flags] <block> [block...]

Arguments:
  blocks - a space-separated list of one or more block identifiers (required)`

const shortBlocks = "retrieve one or more blocks from the chain or local cache"

const longBlocks = `Purpose:
  Retrieve one or more blocks from the chain or local cache.`

const notesBlocks = `
Notes:
  - Blocks is a space-separated list of values, a start-end range, a special, or any combination.
  - Blocks may be specified as either numbers or hashes.
  - Special blocks are detailed under chifra when --list.
  - With the --logs option, optionally specify one or more --emitter, one or more --topics, either or both.
  - The --logs option is significantly faster if you provide an --emitter and/or a --topic.
  - Multiple topics match on topic0, topic1, and so on, not on different topic0's.
  - For the --logs option, large block ranges may crash the node, use --big_range to specify a larger range.`

func init() {
	var capabilities = caps.Default // Additional global caps for chifra blocks
	// EXISTING_CODE
	capabilities = capabilities.Add(caps.Caching)
	capabilities = capabilities.Add(caps.Raw)
	// EXISTING_CODE

	blocksCmd.Flags().SortFlags = false

	blocksCmd.Flags().BoolVarP(&blocksPkg.GetOptions().Hashes, "hashes", "e", false, "display only transaction hashes, default is to display full transaction detail")
	blocksCmd.Flags().BoolVarP(&blocksPkg.GetOptions().Uncles, "uncles", "c", false, "display uncle blocks (if any) instead of the requested block")
	blocksCmd.Flags().BoolVarP(&blocksPkg.GetOptions().Traces, "traces", "t", false, "export the traces from the block as opposed to the block data")
	blocksCmd.Flags().BoolVarP(&blocksPkg.GetOptions().Uniq, "uniq", "u", false, "display a list of uniq address appearances per transaction")
	blocksCmd.Flags().StringVarP(&blocksPkg.GetOptions().Flow, "flow", "f", "", `for the uniq option only, export only from or to (including trace from or to)
One of [ from | to | reward ]`)
	blocksCmd.Flags().BoolVarP(&blocksPkg.GetOptions().Logs, "logs", "l", false, "display only the logs found in the block(s)")
	blocksCmd.Flags().StringSliceVarP(&blocksPkg.GetOptions().Emitter, "emitter", "m", nil, "for the --logs option only, filter logs to show only those logs emitted by the given address(es)")
	blocksCmd.Flags().StringSliceVarP(&blocksPkg.GetOptions().Topic, "topic", "B", nil, "for the --logs option only, filter logs to show only those with this topic(s)")
	blocksCmd.Flags().BoolVarP(&blocksPkg.GetOptions().Articulate, "articulate", "a", false, "for the --logs option only, articulate the retrieved data if ABIs can be found")
	blocksCmd.Flags().Uint64VarP(&blocksPkg.GetOptions().BigRange, "big_range", "r", 500, "for the --logs option only, allow for block ranges larger than 500")
	blocksCmd.Flags().BoolVarP(&blocksPkg.GetOptions().Count, "count", "U", false, "display the number of the lists of appearances for --addrs or --uniq")
	blocksCmd.Flags().BoolVarP(&blocksPkg.GetOptions().CacheTxs, "cache_txs", "", false, "force a write of the block's transactions to the cache (slow) (hidden)")
	blocksCmd.Flags().BoolVarP(&blocksPkg.GetOptions().CacheTraces, "cache_traces", "", false, "force a write of the block's traces to the cache (slower) (hidden)")
	blocksCmd.Flags().BoolVarP(&blocksPkg.GetOptions().Decache, "decache", "D", false, "removes a block and any transactions or traces in the block from the cache")
	blocksCmd.Flags().Uint64VarP(&blocksPkg.GetOptions().List, "list", "L", 0, "summary list of blocks running backwards from latest block minus num (hidden)")
	blocksCmd.Flags().Uint64VarP(&blocksPkg.GetOptions().ListCount, "list_count", "C", 0, "the number of blocks to report for --list option (hidden)")
	if os.Getenv("TEST_MODE") != "true" {
		blocksCmd.Flags().MarkHidden("cache_txs")
		blocksCmd.Flags().MarkHidden("cache_traces")
		blocksCmd.Flags().MarkHidden("list")
		blocksCmd.Flags().MarkHidden("list_count")
	}
	globals.InitGlobals(blocksCmd, &blocksPkg.GetOptions().Globals, capabilities)

	blocksCmd.SetUsageTemplate(UsageWithNotes(notesBlocks))
	blocksCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(blocksCmd)
}
