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

	"github.com/spf13/cobra"
)

// chunksCmd represents the chunks command
var chunksCmd = &cobra.Command{
	Use:   usageChunks,
	Short: shortChunks,
	Long:  longChunks,
	Run:   runChunks,
	Args:  validateChunksArgs,
}

var usageChunks = `chunks [flags]`

var shortChunks = "manage chunks and bloom filters"

var longChunks = `Purpose:
  Manage chunks and bloom filters.`

var notesChunks = ``

type chunksOptionsType struct {
	list    bool
	check   bool
	extract bool
	stats   bool
}

var ChunksOpts chunksOptionsType

func init() {
	chunksCmd.SetOut(os.Stderr)

	chunksCmd.Flags().SortFlags = false
	chunksCmd.PersistentFlags().SortFlags = false
	chunksCmd.Flags().BoolVarP(&ChunksOpts.list, "list", "l", false, "list the bloom and index hashes from local cache or IPFS")
	chunksCmd.Flags().BoolVarP(&ChunksOpts.check, "check", "c", false, "check the validity of the chunk or bloom")
	chunksCmd.Flags().BoolVarP(&ChunksOpts.extract, "extract", "e", false, "show the contents of the chunk or bloom filters")
	chunksCmd.Flags().BoolVarP(&ChunksOpts.stats, "stats", "s", false, "for the --list option only, display statistics about each chunk or bloom")
	chunksCmd.Flags().SortFlags = false
	chunksCmd.PersistentFlags().SortFlags = false

	chunksCmd.SetUsageTemplate(UsageWithNotes(notesChunks))
	rootCmd.AddCommand(chunksCmd)
}

// EXISTING_CODE
// EXISTING_CODE
