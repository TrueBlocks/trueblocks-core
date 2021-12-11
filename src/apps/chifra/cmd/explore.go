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
	Version: "GHC-TrueBlocks//0.18.0-alpha",
	RunE:    explorePkg.RunExplore,
}

var usageExplore = `explore [flags] <term> [term...]

Arguments:
  terms - one or more address, name, block, or transaction identifier`

var shortExplore = "open a local or remote explorer for one or more addresses, blocks, or transactions"

var longExplore = `Purpose:
  Open a local or remote explorer for one or more addresses, blocks, or transactions.`

var notesExplore = ``

func init() {
	exploreCmd.Flags().SortFlags = false

	exploreCmd.Flags().BoolVarP(&explorePkg.Options.Local, "local", "l", false, "open the local TrueBlocks explorer")
	exploreCmd.Flags().BoolVarP(&explorePkg.Options.Google, "google", "g", false, "search google excluding popular blockchain explorers")
	globals.InitGlobals(exploreCmd, &explorePkg.Options.Globals)

	exploreCmd.SetUsageTemplate(UsageWithNotes(notesExplore))
	exploreCmd.SetOut(os.Stderr)

	chifraCmd.AddCommand(exploreCmd)
}
