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
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// blocksCmd represents the blocks command
var blocksCmd = &cobra.Command{
	Use:     usageBlocks,
	Short:   shortBlocks,
	Long:    longBlocks,
	Version: versionText,
	RunE:    blocksPkg.RunBlocks,
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
  - Large block ranges may crash the node, use --big_range to specify a larger range.`

func init() {
	blocksCmd.Flags().SortFlags = false

	blocksCmd.Flags().BoolVarP(&blocksPkg.GetOptions().Hashes, "hashes", "e", false, "display only transaction hashes, default is to display full transaction detail")
	blocksCmd.Flags().BoolVarP(&blocksPkg.GetOptions().Uncles, "uncles", "c", false, "display uncle blocks (if any) instead of the requested block")
	blocksCmd.Flags().BoolVarP(&blocksPkg.GetOptions().Trace, "trace", "t", false, "export the traces from the block as opposed to the block data")
	blocksCmd.Flags().BoolVarP(&blocksPkg.GetOptions().Apps, "apps", "s", false, "display a list of uniq address appearances in the block")
	blocksCmd.Flags().BoolVarP(&blocksPkg.GetOptions().Uniq, "uniq", "u", false, "display a list of uniq address appearances per transaction")
	blocksCmd.Flags().BoolVarP(&blocksPkg.GetOptions().Logs, "logs", "g", false, "display only the logs found in the block(s) (hidden)")
	blocksCmd.Flags().StringSliceVarP(&blocksPkg.GetOptions().Emitter, "emitter", "m", nil, "for the --logs option only, filter logs to show only those logs emitted by the given address(es) (hidden)")
	blocksCmd.Flags().StringSliceVarP(&blocksPkg.GetOptions().Topic, "topic", "p", nil, "for the --logs option only, filter logs to show only those with this topic(s) (hidden)")
	blocksCmd.Flags().BoolVarP(&blocksPkg.GetOptions().Articulate, "articulate", "a", false, "for the --logs option only, articulate the retrieved data if ABIs can be found (hidden)")
	blocksCmd.Flags().Uint64VarP(&blocksPkg.GetOptions().BigRange, "big_range", "r", 500, "for the --logs option only, allow for block ranges larger than 500 (hidden)")
	blocksCmd.Flags().BoolVarP(&blocksPkg.GetOptions().Count, "count", "U", false, "display the number of the lists of appearances for --addrs or --uniq")
	blocksCmd.Flags().BoolVarP(&blocksPkg.GetOptions().Cache, "cache", "o", false, "force a write of the block to the cache")
	blocksCmd.Flags().Uint64VarP(&blocksPkg.GetOptions().List, "list", "l", 0, "summary list of blocks running backwards from latest block minus num (hidden)")
	blocksCmd.Flags().Uint64VarP(&blocksPkg.GetOptions().ListCount, "list_count", "C", 0, "the number of blocks to report for --list option (hidden)")
	if os.Getenv("TEST_MODE") != "true" {
		blocksCmd.Flags().MarkHidden("logs")
		blocksCmd.Flags().MarkHidden("emitter")
		blocksCmd.Flags().MarkHidden("topic")
		blocksCmd.Flags().MarkHidden("articulate")
		blocksCmd.Flags().MarkHidden("big_range")
		blocksCmd.Flags().MarkHidden("list")
		blocksCmd.Flags().MarkHidden("list_count")
	}
	globals.InitGlobals(blocksCmd, &blocksPkg.GetOptions().Globals)

	blocksCmd.SetUsageTemplate(UsageWithNotes(notesBlocks))
	blocksCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(blocksCmd)
}
