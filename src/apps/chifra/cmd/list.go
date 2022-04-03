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
	listPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/list"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// listCmd represents the list command
var listCmd = &cobra.Command{
	Use:     usageList,
	Short:   shortList,
	Long:    longList,
	Version: versionText,
	RunE:    listPkg.RunList,
}

var usageList = `list [flags] <address> [address...]

Arguments:
  addrs - one or more addresses (0x...) to list (required)`

var shortList = "list every appearance of an address anywhere on the chain"

var longList = `Purpose:
  List every appearance of an address anywhere on the chain.`

var notesList = ``

func init() {
	listCmd.Flags().SortFlags = false

	listCmd.Flags().BoolVarP(&listPkg.GetOptions().Count, "count", "U", false, "present only the number of records")
	listCmd.Flags().BoolVarP(&listPkg.GetOptions().Appearances, "appearances", "p", false, "export a list of appearances (hidden)")
	listCmd.Flags().Uint64VarP(&listPkg.GetOptions().FirstBlock, "first_block", "F", 0, "first block to process (inclusive) (hidden)")
	listCmd.Flags().Uint64VarP(&listPkg.GetOptions().LastBlock, "last_block", "L", 0, "last block to process (inclusive) (hidden)")
	if os.Getenv("TEST_MODE") != "true" {
		listCmd.Flags().MarkHidden("appearances")
		listCmd.Flags().MarkHidden("first_block")
		listCmd.Flags().MarkHidden("last_block")
	}
	globals.InitGlobals(listCmd, &listPkg.GetOptions().Globals)

	listCmd.SetUsageTemplate(UsageWithNotes(notesList))
	listCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(listCmd)
}
