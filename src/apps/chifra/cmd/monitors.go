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

const longMonitors = `Purpose:
  Add, remove, clean, and list address monitors.`

const notesMonitors = `
Notes:
  - An address must be either an ENS name or start with '0x' and be forty-two characters long.
  - If no address is presented to the --clean command, all existing monitors will be cleaned.`

func init() {
	var capabilities caps.Capability // capabilities for chifra monitors
	capabilities = capabilities.Add(caps.Default)
	capabilities = capabilities.Add(caps.Caching)
	capabilities = capabilities.Add(caps.Names)

	monitorsCmd.Flags().SortFlags = false

	monitorsCmd.Flags().BoolVarP(&monitorsPkg.GetOptions().Delete, "delete", "", false, `delete a monitor, but do not remove it`)
	monitorsCmd.Flags().BoolVarP(&monitorsPkg.GetOptions().Undelete, "undelete", "", false, `undelete a previously deleted monitor`)
	monitorsCmd.Flags().BoolVarP(&monitorsPkg.GetOptions().Remove, "remove", "", false, `remove a previously deleted monitor`)
	monitorsCmd.Flags().BoolVarP(&monitorsPkg.GetOptions().Clean, "clean", "C", false, `clean (i.e. remove duplicate appearances) from monitors, optionally clear stage`)
	monitorsCmd.Flags().BoolVarP(&monitorsPkg.GetOptions().List, "list", "l", false, `list monitors in the cache (--verbose for more detail)`)
	monitorsCmd.Flags().BoolVarP(&monitorsPkg.GetOptions().Count, "count", "U", false, `show the number of active monitors (included deleted but not removed monitors)`)
	monitorsCmd.Flags().BoolVarP(&monitorsPkg.GetOptions().Staged, "staged", "S", false, `for --clean, --list, and --count options only, include staged monitors`)
	globals.InitGlobals("monitors", monitorsCmd, &monitorsPkg.GetOptions().Globals, capabilities)

	monitorsCmd.SetUsageTemplate(UsageWithNotes(notesMonitors))
	monitorsCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(monitorsCmd)
}
