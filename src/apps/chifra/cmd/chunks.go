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

	chunksPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/chunks"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// chunksCmd represents the chunks command
var chunksCmd = &cobra.Command{
	Use:     usageChunks,
	Short:   shortChunks,
	Long:    longChunks,
	Version: versionText,
	RunE:    chunksPkg.RunChunks,
}

const usageChunks = `chunks <mode> [flags] [blocks...] [address...]

Arguments:
  mode - the type of chunk info to retrieve (required)
	One of [ stats | pins | blooms | index | addresses | appearances ]
  blocks - optional list of blocks to intersect with chunk ranges
  addrs - one or more addresses to use with --belongs option (see note)`

const shortChunks = "manage and investigate chunks and bloom filters"

const longChunks = `Purpose:
  Manage and investigate chunks and bloom filters.`

const notesChunks = `
Notes:
  - If blocks are provided, only chunks intersecting with those blocks are displayed.
  - Only a single block in a given chunk needs to be supplied for a match.
  - The --belongs option is only available with the addresses or blooms mode.
  - The --belongs option requires both an address and a block identifier.
  - You may only specifiy an address when using the --belongs option.`

func init() {
	chunksCmd.Flags().SortFlags = false

	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Check, "check", "c", false, "depends on mode, checks for internal consistency of the data type")
	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Belongs, "belongs", "b", false, "checks if the given address appears in the given chunk")
	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Details, "details", "d", false, "for the --addresses option only, display appearance records as well as address records")
	globals.InitGlobals(chunksCmd, &chunksPkg.GetOptions().Globals)

	chunksCmd.SetUsageTemplate(UsageWithNotes(notesChunks))
	chunksCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(chunksCmd)
}
