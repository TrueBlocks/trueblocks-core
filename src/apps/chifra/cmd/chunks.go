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
	One of [ stats | manifest | pins | blooms | index | addresses | appearances ]
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
  - You may only specifiy an address when using the --belongs option.
  - The two --pin_ options, the --clean option, and the --check option are available only in manifest mode.`

func init() {
	chunksCmd.Flags().SortFlags = false

	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Details, "details", "d", false, "for manifest and addresses options only, display full details of the report")
	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Check, "check", "c", false, "depends on mode, checks for internal consistency of the data type")
	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Belongs, "belongs", "b", false, "checks if the given address appears in the given chunk")
	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().PinChunks, "pin_chunks", "p", false, "gzip each chunk, push it to IPFS, and update and publish the manifest")
	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().PinData, "pin_data", "a", false, "gzip the databases, push them to IPFS, and update and publish the manifest")
	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Clean, "clean", "n", false, "retrieve all pins on Pinata, compare to manifest, remove any extraneous remote pins")
	globals.InitGlobals(chunksCmd, &chunksPkg.GetOptions().Globals)

	chunksCmd.SetUsageTemplate(UsageWithNotes(notesChunks))
	chunksCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(chunksCmd)
}
