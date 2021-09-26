package cmd

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
 * Parts of this file were generated with makeClass --gocmds.
 */

import (
	"errors"
	"fmt"
	"os"

	"github.com/spf13/cobra"
)

// blocksCmd represents the blocks command
var blocksCmd = &cobra.Command{
	Use:   usageBlocks,
	Short: shortBlocks,
	Long:  longBlocks,
	Run:   runBlocks,
	Args:  ValidatePositionals(validateBlocksArgs),
}

var usageBlocks = `blocks [flags] <block> [block...]

Arguments:
  blocks - a space-separated list of one or more block identifiers (required)`

var shortBlocks = "retrieve one or more blocks from the chain or local cache"

var longBlocks = `Purpose:
  Retrieve one or more blocks from the chain or local cache.`

type blocksOptionsType struct {
	hashes     bool
	uncles     bool
	trace      bool
	apps       bool
	uniq       bool
	uniq_tx    bool
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
	blocksCmd.Flags().BoolVarP(&BlocksOpts.apps, "apps", "a", false, "display only the list of address appearances in the block")
	blocksCmd.Flags().BoolVarP(&BlocksOpts.uniq, "uniq", "u", false, "display only the list of uniq address appearances in the block")
	blocksCmd.Flags().BoolVarP(&BlocksOpts.uniq_tx, "uniq_tx", "n", false, "display only the list of uniq address appearances in each transaction")
	blocksCmd.Flags().BoolVarP(&BlocksOpts.count, "count", "c", false, "display the number of the lists of appearances for --apps, --uniq, or --uniq_tx")
	blocksCmd.Flags().BoolVarP(&BlocksOpts.cache, "cache", "o", false, "force a write of the block to the cache")
	blocksCmd.Flags().Uint64VarP(&BlocksOpts.list, "list", "l", 0, "summary list of blocks running backwards from latest block minus num (hidden)")
	blocksCmd.Flags().Uint64VarP(&BlocksOpts.list_count, "list_count", "C", 0, "the number of blocks to report for --list option (hidden)")
	blocksCmd.Flags().SortFlags = false
	blocksCmd.PersistentFlags().SortFlags = false

	PostNotes = ""
	rootCmd.AddCommand(blocksCmd)
}

func runBlocks(cmd *cobra.Command, args []string) {
	options := ""
	if BlocksOpts.hashes {
		options += " --hashes"
	}
	if BlocksOpts.uncles {
		options += " --uncles"
	}
	if BlocksOpts.trace {
		options += " --trace"
	}
	if BlocksOpts.apps {
		options += " --apps"
	}
	if BlocksOpts.uniq {
		options += " --uniq"
	}
	if BlocksOpts.uniq_tx {
		options += " --uniq_tx"
	}
	if BlocksOpts.count {
		options += " --count"
	}
	if BlocksOpts.cache {
		options += " --cache"
	}
	if BlocksOpts.list > 0 {
		options += " --list " + fmt.Sprintf("%d", BlocksOpts.list)
	}
	if BlocksOpts.list_count > 0 {
		options += " --list_count " + fmt.Sprintf("%d", BlocksOpts.list_count)
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn("/Users/jrush/.local/bin/chifra/getBlocks", options, arguments)
}

func validateBlocksArgs(cmd *cobra.Command, args []string) error {
	if len(args) > 0 && args[0] == "12" {
		return ErrFunc(cmd, errors.New("Invalid argument "+args[0]))
	}
	return nil
}
