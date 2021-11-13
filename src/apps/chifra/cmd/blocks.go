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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/spf13/cobra"
)

// blocksCmd represents the blocks command
var blocksCmd = &cobra.Command{
	Use:   usageBlocks,
	Short: shortBlocks,
	Long:  longBlocks,
	Run:   runBlocks,
	Args:  validateBlocksArgs,
}

var usageBlocks = `blocks [flags] <block> [block...]

Arguments:
  blocks - a space-separated list of one or more block identifiers (required)`

var shortBlocks = "retrieve one or more blocks from the chain or local cache"

var longBlocks = `Purpose:
  Retrieve one or more blocks from the chain or local cache.`

var notesBlocks = `
Notes:
  - blocks is a space-separated list of values, a start-end range, a special, or any combination.
  - blocks may be specified as either numbers or hashes.
  - special blocks are detailed under chifra when --list.
  - With the --logs option, optionally specify one or more --emmitter, one or more --topics, either or both.
  - The --logs option is significantly faster if you provide an --emitter and/or a --topic.
  - Multiple topics match on topic0, topic1, and so on, not on different topic0's.
  - Large block ranges may crash the node, use --big_range to specify a larger range.`

type blocksOptionsType struct {
	hashes     bool
	uncles     bool
	trace      bool
	apps       bool
	uniq       bool
	logs       bool
	emitter    []string
	topic      []string
	articulate bool
	big_range  uint64
	count      bool
	cache      bool
	list       uint64
	list_count uint64
}

var BlocksOpts blocksOptionsType

func init() {
	blocksCmd.SetOut(os.Stderr)

	blocksCmd.Flags().SortFlags = false
	blocksCmd.PersistentFlags().SortFlags = false
	blocksCmd.Flags().BoolVarP(&BlocksOpts.hashes, "hashes", "e", false, "display only transaction hashes, default is to display full transaction detail")
	blocksCmd.Flags().BoolVarP(&BlocksOpts.uncles, "uncles", "U", false, "display uncle blocks (if any) instead of the requested block")
	blocksCmd.Flags().BoolVarP(&BlocksOpts.trace, "trace", "t", false, "export the traces from the block as opposed to the block data")
	blocksCmd.Flags().BoolVarP(&BlocksOpts.apps, "apps", "s", false, "display a list of uniq address appearances in the block")
	blocksCmd.Flags().BoolVarP(&BlocksOpts.uniq, "uniq", "u", false, "display a list of uniq address appearances per transaction")
	blocksCmd.Flags().BoolVarP(&BlocksOpts.logs, "logs", "g", false, "display only the logs found in the block(s) (hidden)")
	blocksCmd.Flags().StringSliceVarP(&BlocksOpts.emitter, "emitter", "m", nil, "for the --logs option only, filter logs to show only those logs emitted by the given address(es) (hidden)")
	blocksCmd.Flags().StringSliceVarP(&BlocksOpts.topic, "topic", "p", nil, "for the --logs option only, filter logs to show only those with this topic(s) (hidden)")
	blocksCmd.Flags().BoolVarP(&BlocksOpts.articulate, "articulate", "a", false, "for the --logs option only, articulate the retrieved data if ABIs can be found (hidden)")
	blocksCmd.Flags().Uint64VarP(&BlocksOpts.big_range, "big_range", "r", 500, "for the --logs option only, allow for block ranges larger than 500 (hidden)")
	blocksCmd.Flags().BoolVarP(&BlocksOpts.count, "count", "c", false, "display the number of the lists of appearances for --addrs or --uniq")
	blocksCmd.Flags().BoolVarP(&BlocksOpts.cache, "cache", "o", false, "force a write of the block to the cache")
	blocksCmd.Flags().Uint64VarP(&BlocksOpts.list, "list", "l", 0, "summary list of blocks running backwards from latest block minus num (hidden)")
	blocksCmd.Flags().Uint64VarP(&BlocksOpts.list_count, "list_count", "C", 20, "the number of blocks to report for --list option (hidden)")
	if !utils.IsTestMode() {
		blocksCmd.Flags().MarkHidden("logs")
		blocksCmd.Flags().MarkHidden("emitter")
		blocksCmd.Flags().MarkHidden("topic")
		blocksCmd.Flags().MarkHidden("articulate")
		blocksCmd.Flags().MarkHidden("big_range")
		blocksCmd.Flags().MarkHidden("list")
		blocksCmd.Flags().MarkHidden("list_count")
	}
	blocksCmd.Flags().SortFlags = false
	blocksCmd.PersistentFlags().SortFlags = false

	blocksCmd.SetUsageTemplate(UsageWithNotes(notesBlocks))
	rootCmd.AddCommand(blocksCmd)
}

func TestLogBlocks(args []string) {
	if !utils.IsTestMode() {
		return
	}
}

// EXISTING_CODE
// EXISTING_CODE
