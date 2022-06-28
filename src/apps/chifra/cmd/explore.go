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
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// exploreCmd represents the explore command
var exploreCmd = &cobra.Command{
	Use:     usageExplore,
	Short:   shortExplore,
	Long:    longExplore,
	Version: versionText,
	RunE:    explorePkg.RunExplore,
}

const usageExplore = `explore [flags] <term> [term...]

Arguments:
  terms - one or more address, name, block, or transaction identifier`

const shortExplore = "open a local or remote explorer for one or more addresses, blocks, or transactions"

const longExplore = `Purpose:
  Open a local or remote explorer for one or more addresses, blocks, or transactions.`

const notesExplore = ``

func init() {
	exploreCmd.Flags().SortFlags = false

	exploreCmd.Flags().BoolVarP(&explorePkg.GetOptions().Local, "local", "l", false, "open the local TrueBlocks explorer")
	exploreCmd.Flags().BoolVarP(&explorePkg.GetOptions().Google, "google", "g", false, "search google excluding popular blockchain explorers")
	globals.InitGlobals(exploreCmd, &explorePkg.GetOptions().Globals)

	exploreCmd.SetUsageTemplate(UsageWithNotes(notesExplore))
	exploreCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	exploreCmd.Flags().MarkHidden("verbose")
	exploreCmd.Flags().MarkHidden("fmt")
	// EXISTING_CODE

	chifraCmd.AddCommand(exploreCmd)
}
