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
  mode - the type of information to retrieve (required)
	One of [ status | index | blooms | manifest | stats | addresses | appearances ]
  blocks - optional list of blocks used to intersect with chunk ranges
  addrs - optional list of addresses for use with --belongs option (see notes)`

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
  - The two --pin_ options, the --clean option, and the --check option are available only in manifest mode.
  - The --repair and --reset options also update the manifest, but do not publish it.`

func init() {
	chunksCmd.Flags().SortFlags = false

	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Pin, "pin", "i", false, "pin all chunks (locally if IPFS daemon is running, and/or remotely with --remote flag)")
	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Remote, "remote", "m", false, "for some options, forces processing to use remote data")
	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Publish, "publish", "p", false, "repin chunks, pin the manifest, and publish to the Unchained Index smart contract")
	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Check, "check", "c", false, "depends on mode, checks for internal consistency of the given type")
	chunksCmd.Flags().Uint64VarP(&chunksPkg.GetOptions().Reset, "reset", "r", 0, "in index mode only, removes chunks inclusive of or after this block identifier (hidden)")
	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Repair, "repair", "e", false, "in index mode only, repair a chunk (requires block identifier) (hidden)")
	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Belongs, "belongs", "b", false, "in index mode only, checks if the given address appears in the given chunk")
	if os.Getenv("TEST_MODE") != "true" {
		chunksCmd.Flags().MarkHidden("reset")
		chunksCmd.Flags().MarkHidden("repair")
	}
	globals.InitGlobals(chunksCmd, &chunksPkg.GetOptions().Globals)

	chunksCmd.SetUsageTemplate(UsageWithNotes(notesChunks))
	chunksCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(chunksCmd)
}
