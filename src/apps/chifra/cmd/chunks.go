// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package cmd

// EXISTING_CODE
import (
	"os"

	chunksPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/internal/chunks"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// chunksCmd represents the chunks command
var chunksCmd = &cobra.Command{
	Use:     usageChunks,
	Long:    longChunks,
	Version: versionText,
	PreRun: outputHelpers.PreRunWithJsonWriter("chunks", func() *globals.GlobalOptions {
		return &chunksPkg.GetOptions().Globals
	}),
	RunE: file.RunWithFileSupport("chunks", chunksPkg.RunChunks, chunksPkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &chunksPkg.GetOptions().Globals
	}),
}

const usageChunks = `chunks <mode> [flags] [blocks...] [address...]

Arguments:
  mode - the type of data to process (required)
    One of [ manifest | index | blooms | pins | addresses | appearances | stats ]
  blocks - an optional list of blocks to intersect with chunk ranges`

const longChunks = `Purpose:
  Manage, investigate, and display the Unchained Index.`

const notesChunks = `
Notes:
  - Available modes: stats, index, blooms, manifest, pins, addresses, appearances.
  - Some options are mode-specific (e.g., --belongs only works in index mode).
  - Block filters limit results to intersecting chunks only.
  - To pin only metadata (ts.bin and manifest.json), use --pin --metadata.
  - The --truncate option updates the manifest and removes local data, but does not alter remote pins.
  - The --belongs option is only available in index mode.
  - Block range options (--first_block, --last_block) apply to addresses, appearances, and index modes.
  - Pinning requires a locally-running IPFS node or pinning service API key.
  - With --publish, transaction sender becomes publisher (that is, the --publisher ignored).
  - Without --rewrite, manifest is written to a temp folder; with --rewrite, to the index folder.`

func init() {
	var capabilities caps.Capability // capabilities for chifra chunks
	capabilities = capabilities.Add(caps.Default)

	chunksCmd.Flags().SortFlags = false

	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Check, "check", "c", false, `check the manifest, index, or blooms for internal consistency`)
	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Pin, "pin", "i", false, `pin the manifest or each index chunk and bloom`)
	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Publish, "publish", "p", false, `publish the manifest to the Unchained Index smart contract`)
	chunksCmd.Flags().StringVarP(&chunksPkg.GetOptions().Publisher, "publisher", "P", "", `for some query options, the publisher of the index (hidden)`)
	chunksCmd.Flags().Uint64VarP((*uint64)(&chunksPkg.GetOptions().Truncate), "truncate", "n", 0, `truncate the entire index at this block (requires a block identifier) (hidden)`)
	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Remote, "remote", "r", false, `prior to processing, retrieve the manifest from the Unchained Index smart contract`)
	chunksCmd.Flags().StringSliceVarP(&chunksPkg.GetOptions().Belongs, "belongs", "b", nil, `in index mode only, checks the address(es) for inclusion in the given index chunk`)
	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Diff, "diff", "f", false, `compare two index portions (see notes) (hidden)`)
	chunksCmd.Flags().Uint64VarP((*uint64)(&chunksPkg.GetOptions().FirstBlock), "first_block", "F", 0, `first block to process (inclusive)`)
	chunksCmd.Flags().Uint64VarP((*uint64)(&chunksPkg.GetOptions().LastBlock), "last_block", "L", 0, `last block to process (inclusive)`)
	chunksCmd.Flags().Uint64VarP(&chunksPkg.GetOptions().MaxAddrs, "max_addrs", "m", 0, `the max number of addresses to process in a given chunk`)
	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Deep, "deep", "D", false, `if true, dig more deeply during checking (manifest only)`)
	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Rewrite, "rewrite", "e", false, `for the --pin --deep mode only, writes the manifest back to the index folder (see notes)`)
	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().List, "list", "l", false, `for the pins mode only, list the remote pins (hidden)`)
	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Unpin, "unpin", "u", false, `for the pins mode only, if true reads local ./unpins file for valid CIDs and remotely unpins each (skips non-CIDs) (hidden)`)
	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Count, "count", "U", false, `for certain modes only, display the count of records`)
	chunksCmd.Flags().StringVarP(&chunksPkg.GetOptions().Tag, "tag", "t", "", `visits each chunk and updates the headers with the supplied version string (vX.Y.Z-str) (hidden)`)
	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().DryRun, "dry_run", "d", false, `show what the command would do without actually making any changes (hidden)`)
	chunksCmd.Flags().BoolVarP(&chunksPkg.GetOptions().Metadata, "metadata", "M", false, `for --pin only, pin only metadata files (ts.bin and manifest.json)`)
	chunksCmd.Flags().Float64VarP(&chunksPkg.GetOptions().Sleep, "sleep", "s", 0.0, `for --remote pinning only, seconds to sleep between API calls`)
	if !base.IsTestMode() {
		_ = chunksCmd.Flags().MarkHidden("publisher")
		_ = chunksCmd.Flags().MarkHidden("truncate")
		_ = chunksCmd.Flags().MarkHidden("diff")
		_ = chunksCmd.Flags().MarkHidden("list")
		_ = chunksCmd.Flags().MarkHidden("unpin")
		_ = chunksCmd.Flags().MarkHidden("tag")
		_ = chunksCmd.Flags().MarkHidden("dry_run")
	}
	globals.InitGlobals("chunks", chunksCmd, &chunksPkg.GetOptions().Globals, capabilities)

	chunksCmd.SetUsageTemplate(UsageWithNotes(notesChunks))
	chunksCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(chunksCmd)
}
