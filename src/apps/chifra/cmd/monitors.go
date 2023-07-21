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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// monitorsCmd represents the monitors command
var monitorsCmd = &cobra.Command{
	Use:     usageMonitors,
	Short:   shortMonitors,
	Long:    longMonitors,
	Version: versionText,
	PreRun: outputHelpers.PreRunWithJsonWriter("monitors", func() *globals.GlobalOptions {
		return &monitorsPkg.GetOptions().Globals
	}),
	RunE: file.RunWithFileSupport("monitors", monitorsPkg.RunMonitors, monitorsPkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &monitorsPkg.GetOptions().Globals
	}),
}

const usageMonitors = `monitors [flags] <address> [address...]

Arguments:
  addrs - one or more addresses (0x...) to process`

const shortMonitors = "add, remove, clean, and list address monitors"

const longMonitors = `Purpose:
  Add, remove, clean, and list address monitors.`

const notesMonitors = `
Notes:
  - An address must be either an ENS name or start with '0x' and be forty-two characters long.
  - If no address is presented to the --clean command, all monitors will be cleaned.
  - The --decache option will remove all cache items (blocks, txs, traces, recons) for the given address(es).`

func init() {
	allowCaching := false
	// EXISTING_CODE
	// EXISTING_CODE

	monitorsCmd.Flags().SortFlags = false

	monitorsCmd.Flags().BoolVarP(&monitorsPkg.GetOptions().Delete, "delete", "", false, "delete a monitor, but do not remove it")
	monitorsCmd.Flags().BoolVarP(&monitorsPkg.GetOptions().Undelete, "undelete", "", false, "undelete a previously deleted monitor")
	monitorsCmd.Flags().BoolVarP(&monitorsPkg.GetOptions().Remove, "remove", "", false, "remove a previously deleted monitor")
	monitorsCmd.Flags().BoolVarP(&monitorsPkg.GetOptions().Clean, "clean", "C", false, "clean (i.e. remove duplicate appearances) from monitors")
	monitorsCmd.Flags().BoolVarP(&monitorsPkg.GetOptions().Decache, "decache", "D", false, "removes a monitor and all associated data from the cache")
	monitorsCmd.Flags().BoolVarP(&monitorsPkg.GetOptions().List, "list", "l", false, "list monitors in the cache (--verbose for more detail)")
	monitorsCmd.Flags().BoolVarP(&monitorsPkg.GetOptions().Watch, "watch", "w", false, "continually scan for new blocks and extract data for monitored addresses")
	monitorsCmd.Flags().Float64VarP(&monitorsPkg.GetOptions().Sleep, "sleep", "s", 14, "seconds to sleep between monitor passes")
	globals.InitGlobals(monitorsCmd, &monitorsPkg.GetOptions().Globals, allowCaching)

	monitorsCmd.SetUsageTemplate(UsageWithNotes(notesMonitors))
	monitorsCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(monitorsCmd)
}
