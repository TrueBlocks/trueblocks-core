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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	initPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/init"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// initCmd represents the init command
var initCmd = &cobra.Command{
	Use:     usageInit,
	Short:   shortInit,
	Long:    longInit,
	Version: versionText,
	PreRun: outputHelpers.PreRunWithJsonWriter("init", func() *globals.GlobalOptions {
		return &initPkg.GetOptions().Globals
	}),
	RunE: file.RunWithFileSupport("init", initPkg.RunInit, initPkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &initPkg.GetOptions().Globals
	}),
}

const usageInit = `init [flags]`

const shortInit = "initialize the TrueBlocks system by downloading the Unchained Index from IPFS"

const longInit = `Purpose:
  Initialize the TrueBlocks system by downloading the Unchained Index from IPFS.`

const notesInit = `
Notes:
  - If run with no options, this tool will download or freshen only the Bloom filters.
  - The --first_block option will fall back to the start of the containing chunk.
  - You may re-run the tool as often as you wish. It will repair or freshen the index.`

func init() {
	var capabilities = caps.Default // Additional global caps for chifra init
	// EXISTING_CODE
	capabilities = capabilities.Remove(caps.Fmt)
	capabilities = capabilities.Remove(caps.NoHeader)
	capabilities = capabilities.Remove(caps.File)
	capabilities = capabilities.Remove(caps.Output)
	capabilities = capabilities.Remove(caps.Append)
	// EXISTING_CODE

	initCmd.Flags().SortFlags = false

	initCmd.Flags().BoolVarP(&initPkg.GetOptions().All, "all", "a", false, "in addition to Bloom filters, download full index chunks (recommended)")
	initCmd.Flags().BoolVarP(&initPkg.GetOptions().DryRun, "dry_run", "d", false, "display the results of the download without actually downloading")
	initCmd.Flags().StringVarP(&initPkg.GetOptions().Publisher, "publisher", "P", "trueblocks.eth", "the publisher of the index to download")
	initCmd.Flags().Uint64VarP(&initPkg.GetOptions().FirstBlock, "first_block", "F", 0, "do not download any chunks earlier than this block")
	initCmd.Flags().Float64VarP(&initPkg.GetOptions().Sleep, "sleep", "s", 0.0, "seconds to sleep between downloads")
	globals.InitGlobals(initCmd, &initPkg.GetOptions().Globals, capabilities)

	initCmd.SetUsageTemplate(UsageWithNotes(notesInit))
	initCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(initCmd)
}
