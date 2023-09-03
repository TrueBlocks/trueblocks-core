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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
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
  - If no address is presented to the --clean command, all existing monitors will be cleaned.
  - The --watch option requires two additional parameters to be specified: --watchlist and --commands.
  - Addresses provided on the command line are ignored in --watch mode.
  - Providing the value existing to the --watchlist monitors all existing monitor files (see --list).`

func init() {
	var capabilities = caps.Default // Additional global caps for chifra monitors
	// EXISTING_CODE
	// EXISTING_CODE

	monitorsCmd.Flags().SortFlags = false

	monitorsCmd.Flags().BoolVarP(&monitorsPkg.GetOptions().Delete, "delete", "", false, "delete a monitor, but do not remove it")
	monitorsCmd.Flags().BoolVarP(&monitorsPkg.GetOptions().Undelete, "undelete", "", false, "undelete a previously deleted monitor")
	monitorsCmd.Flags().BoolVarP(&monitorsPkg.GetOptions().Remove, "remove", "", false, "remove a previously deleted monitor")
	monitorsCmd.Flags().BoolVarP(&monitorsPkg.GetOptions().Clean, "clean", "C", false, "clean (i.e. remove duplicate appearances) from monitors")
	monitorsCmd.Flags().BoolVarP(&monitorsPkg.GetOptions().List, "list", "l", false, "list monitors in the cache (--verbose for more detail)")
	monitorsCmd.Flags().BoolVarP(&monitorsPkg.GetOptions().Watch, "watch", "w", false, "continually scan for new blocks and extract data as per the command file")
	monitorsCmd.Flags().StringVarP(&monitorsPkg.GetOptions().Watchlist, "watchlist", "a", "", "available with --watch option only, a file containing the addresses to watch")
	monitorsCmd.Flags().StringVarP(&monitorsPkg.GetOptions().Commands, "commands", "c", "", "available with --watch option only, the file containing the list of commands to apply to each watched address")
	monitorsCmd.Flags().Uint64VarP(&monitorsPkg.GetOptions().BatchSize, "batch_size", "b", 8, "available with --watch option only, the number of monitors to process in each batch")
	monitorsCmd.Flags().Uint64VarP(&monitorsPkg.GetOptions().RunCount, "run_count", "u", 0, "available with --watch option only, run the monitor this many times, then quit (hidden)")
	monitorsCmd.Flags().Float64VarP(&monitorsPkg.GetOptions().Sleep, "sleep", "s", 14, "available with --watch option only, the number of seconds to sleep between runs")
	if os.Getenv("TEST_MODE") != "true" {
		monitorsCmd.Flags().MarkHidden("run_count")
	}
	globals.InitGlobals(monitorsCmd, &monitorsPkg.GetOptions().Globals, capabilities)

	monitorsCmd.SetUsageTemplate(UsageWithNotes(notesMonitors))
	monitorsCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(monitorsCmd)
}
