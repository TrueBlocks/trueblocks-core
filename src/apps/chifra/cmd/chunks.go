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

	chunksPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/chunks"
	"github.com/spf13/cobra"
)

// chunksCmd represents the chunks command
var chunksCmd = &cobra.Command{
	Use:   usageChunks,
	Short: shortChunks,
	Long:  longChunks,
	RunE:  chunksPkg.Run,
	Args:  chunksPkg.Validate,
}

var usageChunks = `chunks [flags] <block> [block...]

Arguments:
  blocks - an optional list of blocks to process`

var shortChunks = "manage and investigate chunks and bloom filters"

var longChunks = `Purpose:
  Manage and investigate chunks and bloom filters.`

var notesChunks = `
Notes:
  - Only a single block in a given chunk needs to be supplied.`

func init() {
	chunksCmd.SetOut(os.Stderr)

	chunksCmd.Flags().SortFlags = false
	chunksCmd.PersistentFlags().SortFlags = false
	chunksCmd.Flags().BoolVarP(&chunksPkg.Options.List, "list", "l", false, "list the bloom and index hashes from local cache or IPFS")
	chunksCmd.Flags().BoolVarP(&chunksPkg.Options.Check, "check", "c", false, "check the validity of the chunk or bloom")
	chunksCmd.Flags().StringVarP(&chunksPkg.Options.Extract, "extract", "e", "", `show some or all of the contents of the chunk or bloom filters
One of [ header | addr_table | app_table | chunks | blooms ]`)
	chunksCmd.Flags().BoolVarP(&chunksPkg.Options.Stats, "stats", "s", false, "for the --list option only, display statistics about each chunk or bloom")
	chunksCmd.Flags().BoolVarP(&chunksPkg.Options.Save, "save", "a", false, "for the --extract option only, save the entire chunk to a similarly named file as well as display")
	chunksCmd.Flags().SortFlags = false
	chunksCmd.PersistentFlags().SortFlags = false

	chunksCmd.SetUsageTemplate(UsageWithNotes(notesChunks))
	rootCmd.AddCommand(chunksCmd)
}
