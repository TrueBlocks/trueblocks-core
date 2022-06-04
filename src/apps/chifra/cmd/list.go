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

const usageList = `list [flags] <address> [address...]

Arguments:
  addrs - one or more addresses (0x...) to list (required)`

const shortList = "list every appearance of an address anywhere on the chain"

const longList = `Purpose:
  List every appearance of an address anywhere on the chain.`

const notesList = `
Notes:
  - No other options are permitted when --silent is selected.`

func init() {
	listCmd.Flags().SortFlags = false

	listCmd.Flags().BoolVarP(&listPkg.GetOptions().Count, "count", "U", false, "display only the count of records for each monitor")
	listCmd.Flags().BoolVarP(&listPkg.GetOptions().Appearances, "appearances", "p", false, "export each monitor's list of appearances (the default) (hidden)")
	listCmd.Flags().BoolVarP(&listPkg.GetOptions().Silent, "silent", "", false, "freshen the monitor only (no reporting) (hidden)")
	listCmd.Flags().Uint64VarP(&listPkg.GetOptions().FirstBlock, "first_block", "F", 0, "first block to export (inclusive, ignored when counting or freshening) (hidden)")
	listCmd.Flags().Uint64VarP(&listPkg.GetOptions().LastBlock, "last_block", "L", 0, "last block to export (inclusive, ignored when counting or freshening) (hidden)")
	if os.Getenv("TEST_MODE") != "true" {
		listCmd.Flags().MarkHidden("appearances")
		listCmd.Flags().MarkHidden("silent")
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
