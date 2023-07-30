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

	explorePkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/explore"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// exploreCmd represents the explore command
var exploreCmd = &cobra.Command{
	Use:     usageExplore,
	Short:   shortExplore,
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

const usageExplore = `explore [flags] <term> [term...]

Arguments:
  terms - one or more address, name, block, or transaction identifier`

const shortExplore = "open a local or remote explorer for one or more addresses, blocks, or transactions"

const longExplore = `Purpose:
  Open a local or remote explorer for one or more addresses, blocks, or transactions.`

const notesExplore = ``

func init() {
	var capabilities = caps.Default // Additional global caps for chifra explore
	// EXISTING_CODE
	capabilities = capabilities.Remove(caps.Fmt)
	capabilities = capabilities.Remove(caps.NoHeader)
	capabilities = capabilities.Remove(caps.Output)
	capabilities = capabilities.Remove(caps.Append)
	// EXISTING_CODE

	exploreCmd.Flags().SortFlags = false

	exploreCmd.Flags().BoolVarP(&explorePkg.GetOptions().Local, "local", "l", false, "open the local TrueBlocks explorer")
	exploreCmd.Flags().BoolVarP(&explorePkg.GetOptions().Google, "google", "g", false, "search google excluding popular blockchain explorers")
	globals.InitGlobals(exploreCmd, &explorePkg.GetOptions().Globals, capabilities)

	exploreCmd.SetUsageTemplate(UsageWithNotes(notesExplore))
	exploreCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	exploreCmd.Flags().MarkHidden("verbose")
	exploreCmd.Flags().MarkHidden("fmt")
	// EXISTING_CODE

	chifraCmd.AddCommand(exploreCmd)
}
