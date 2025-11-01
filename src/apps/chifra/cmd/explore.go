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

	explorePkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/internal/explore"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// exploreCmd represents the explore command
var exploreCmd = &cobra.Command{
	Use:     usageExplore,
	Long:    longExplore,
	Version: versionText,
	PreRun: outputHelpers.PreRunWithJsonWriter("explore", func() *globals.GlobalOptions {
		return &explorePkg.GetOptions().Globals
	}),
	RunE: file.RunWithFileSupport("explore", explorePkg.RunExplore, explorePkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &explorePkg.GetOptions().Globals
	}),
}

const usageExplore = `explore [flags] [terms...]

Arguments:
  terms - one or more address, name, block, or transaction identifier`

const longExplore = `Purpose:
  Open a local or remote explorer for one or more addresses, blocks, or transactions.`

const notesExplore = ``

func init() {
	var capabilities caps.Capability // capabilities for chifra explore
	capabilities = capabilities.Add(caps.Default)

	exploreCmd.Flags().SortFlags = false

	exploreCmd.Flags().BoolVarP(&explorePkg.GetOptions().NoOpen, "no_open", "n", false, `return the URL without opening it`)
	exploreCmd.Flags().BoolVarP(&explorePkg.GetOptions().Local, "local", "l", false, `open the local TrueBlocks explorer`)
	exploreCmd.Flags().BoolVarP(&explorePkg.GetOptions().Google, "google", "g", false, `search google excluding popular blockchain explorers`)
	exploreCmd.Flags().BoolVarP(&explorePkg.GetOptions().Dalle, "dalle", "d", false, `open the address to the DalleDress explorer (hidden)`)
	if !base.IsTestMode() {
		_ = exploreCmd.Flags().MarkHidden("dalle")
	}
	globals.InitGlobals("explore", exploreCmd, &explorePkg.GetOptions().Globals, capabilities)

	exploreCmd.SetUsageTemplate(UsageWithNotes(notesExplore))
	exploreCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	_ = exploreCmd.Flags().MarkHidden("verbose")
	_ = exploreCmd.Flags().MarkHidden("fmt")
	// EXISTING_CODE

	chifraCmd.AddCommand(exploreCmd)
}
