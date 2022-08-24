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
  mode - the type of data to process (required)
	One of [ status | manifest | index | blooms | addresses | appearances | stats ]
  blocks - an optional list of blocks to intersect with chunk ranges`

const shortChunks = "manage, investigate, and display the Unchained Index"

const longChunks = `Purpose:
  Manage, investigate, and display the Unchained Index.`

const notesChunks = `
Notes:
  - Mode determines which type of data to display or process.
  - Certain options are only available in certain modes.
  - If blocks are provided, only chunks intersecting with those blocks are displayed.
  - The --repair and --truncate options update data, but do not --pin or --publish.
  - You may combine the --pin and --publish options.
  - The --belongs option is only available in the index mode.`

func init() {
	chunksCmd.Flags().SortFlags = false

	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Check, "check", "c", false, "check the manifest, index, or blooms for internal consistency")
	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Repair, "repair", "r", false, "repair the manifest or a single index chunk (index repair requires a block identifier)")
	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Pin, "pin", "i", false, "pin the manifest or each index chunk and bloom")
	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Publish, "publish", "p", false, "publish the manifest to the Unchained Index smart contract")
	chunksCmd.Flags().Uint64VarP(&chunksPkg.GetOptions().Truncate, "truncate", "n", 0, "truncate the entire index at this block (requires a block identifier)")
	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Remote, "remote", "m", false, "prior to processing, retreive the manifest from the Unchained Index smart contract")
	chunksCmd.Flags().StringSliceVarP(&chunksPkg.GetOptions().Belongs, "belongs", "b", nil, "in index mode only, checks the address(es) for inclusion in the given index chunk")
	chunksCmd.Flags().Float64VarP(&chunksPkg.GetOptions().Settings.Sleep, "sleep", "", 0.0, "for --remote pinning only, seconds to sleep between API calls (hidden)")
	if os.Getenv("TEST_MODE") != "true" {
		chunksCmd.Flags().MarkHidden("sleep")
	}
	globals.InitGlobals(chunksCmd, &chunksPkg.GetOptions().Globals)

	chunksCmd.SetUsageTemplate(UsageWithNotes(notesChunks))
	chunksCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(chunksCmd)
}
