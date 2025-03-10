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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	initPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/init"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// initCmd represents the init command
var initCmd = &cobra.Command{
	Use:     usageInit,
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

const longInit = `Purpose:
  Initialize the TrueBlocks system by downloading the Unchained Index from IPFS.`

const notesInit = `
Notes:
  - If run with no options, this tool will download or freshen only the Bloom filters.
  - The --first_block option will fall back to the start of the containing chunk.
  - You may re-run the tool as often as you wish. It will repair or freshen the index.`

func init() {
	var capabilities caps.Capability // capabilities for chifra init
	capabilities = capabilities.Add(caps.Verbose)
	capabilities = capabilities.Add(caps.Version)
	capabilities = capabilities.Add(caps.Noop)
	capabilities = capabilities.Add(caps.NoColor)
	capabilities = capabilities.Add(caps.Chain)

	initCmd.Flags().SortFlags = false

	initCmd.Flags().BoolVarP(&initPkg.GetOptions().All, "all", "a", false, `in addition to Bloom filters, download full index chunks (recommended)`)
	initCmd.Flags().StringVarP(&initPkg.GetOptions().Example, "example", "e", "", `create an example for the SDK with the given name`)
	initCmd.Flags().BoolVarP(&initPkg.GetOptions().DryRun, "dry_run", "d", false, `display the results of the download without actually downloading`)
	initCmd.Flags().StringVarP(&initPkg.GetOptions().Publisher, "publisher", "P", "", `the publisher of the index to download (hidden)`)
	initCmd.Flags().Uint64VarP((*uint64)(&initPkg.GetOptions().FirstBlock), "first_block", "F", 0, `do not download any chunks earlier than this block`)
	initCmd.Flags().Float64VarP(&initPkg.GetOptions().Sleep, "sleep", "s", 0.0, `seconds to sleep between downloads`)
	if !base.IsTestMode() {
		_ = initCmd.Flags().MarkHidden("publisher")
	}
	globals.InitGlobals("init", initCmd, &initPkg.GetOptions().Globals, capabilities)

	initCmd.SetUsageTemplate(UsageWithNotes(notesInit))
	initCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(initCmd)
}
