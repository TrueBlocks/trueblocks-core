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
	monitorsPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/monitors"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// monitorsCmd represents the monitors command
var monitorsCmd = &cobra.Command{
	Use:     usageMonitors,
	Short:   shortMonitors,
	Long:    longMonitors,
	Version: versionText,
	RunE:    monitorsPkg.RunMonitors,
}

var usageMonitors = `monitors [flags] <address> [address...]

Arguments:
  addrs - one or more addresses (0x...) to process (required)`

var shortMonitors = "add, remove, clean, and list address monitors"

var longMonitors = `Purpose:
  Add, remove, clean, and list address monitors.`

var notesMonitors = `
Notes:
  - An address must start with '0x' and be forty-two characters long.
  - If no address is presented to the --clean command, all monitors will be cleaned.`

func init() {
	monitorsCmd.Flags().SortFlags = false

	monitorsCmd.Flags().BoolVarP(&monitorsPkg.GetOptions().Appearances, "appearances", "p", false, "export a list of appearances")
	monitorsCmd.Flags().BoolVarP(&monitorsPkg.GetOptions().Count, "count", "U", false, "present only the number of records")
	monitorsCmd.Flags().BoolVarP(&monitorsPkg.GetOptions().Clean, "clean", "", false, "clean (i.e. remove duplicate appearances) from monitors")
	monitorsCmd.Flags().BoolVarP(&monitorsPkg.GetOptions().Delete, "delete", "", false, "delete a monitor, but do not remove it")
	monitorsCmd.Flags().BoolVarP(&monitorsPkg.GetOptions().Undelete, "undelete", "", false, "undelete a previously deleted monitor")
	monitorsCmd.Flags().BoolVarP(&monitorsPkg.GetOptions().Remove, "remove", "", false, "remove a previously deleted monitor")
	monitorsCmd.Flags().Uint64VarP(&monitorsPkg.GetOptions().FirstBlock, "first_block", "F", 0, "first block to process (inclusive) (hidden)")
	monitorsCmd.Flags().Uint64VarP(&monitorsPkg.GetOptions().LastBlock, "last_block", "L", 0, "last block to process (inclusive) (hidden)")
	if os.Getenv("TEST_MODE") != "true" {
		monitorsCmd.Flags().MarkHidden("first_block")
		monitorsCmd.Flags().MarkHidden("last_block")
	}
	globals.InitGlobals(monitorsCmd, &monitorsPkg.GetOptions().Globals)

	monitorsCmd.SetUsageTemplate(UsageWithNotes(notesMonitors))
	monitorsCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(monitorsCmd)
}
