/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass --gocmds. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
package cmd

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/spf13/cobra"
)

// monitorsCmd represents the monitors command
var monitorsCmd = &cobra.Command{
	Use:   usageMonitors,
	Short: shortMonitors,
	Long:  longMonitors,
	Run:   runMonitors,
	Args:  validateMonitorsArgs,
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

type monitorsOptionsType struct {
	appearances bool
	count       bool
	clean       bool
	delete      bool
	undelete    bool
	remove      bool
	first_block uint64
	last_block  uint64
}

var MonitorsOpts monitorsOptionsType

func init() {
	monitorsCmd.SetOut(os.Stderr)

	monitorsCmd.Flags().SortFlags = false
	monitorsCmd.PersistentFlags().SortFlags = false
	monitorsCmd.Flags().BoolVarP(&MonitorsOpts.appearances, "appearances", "p", false, "export a list of appearances")
	monitorsCmd.Flags().BoolVarP(&MonitorsOpts.count, "count", "U", false, "present only the number of records")
	monitorsCmd.Flags().BoolVarP(&MonitorsOpts.clean, "clean", "", false, "clean (i.e. remove duplicate appearances) from monitors")
	monitorsCmd.Flags().BoolVarP(&MonitorsOpts.delete, "delete", "", false, "delete a monitor, but do not remove it")
	monitorsCmd.Flags().BoolVarP(&MonitorsOpts.undelete, "undelete", "", false, "undelete a previously deleted monitor")
	monitorsCmd.Flags().BoolVarP(&MonitorsOpts.remove, "remove", "", false, "remove a previously deleted monitor")
	monitorsCmd.Flags().Uint64VarP(&MonitorsOpts.first_block, "first_block", "F", 0, "first block to process (inclusive) (hidden)")
	monitorsCmd.Flags().Uint64VarP(&MonitorsOpts.last_block, "last_block", "L", 0, "last block to process (inclusive) (hidden)")
	if !utils.IsTestMode() {
		monitorsCmd.Flags().MarkHidden("first_block")
		monitorsCmd.Flags().MarkHidden("last_block")
	}
	monitorsCmd.Flags().SortFlags = false
	monitorsCmd.PersistentFlags().SortFlags = false

	monitorsCmd.SetUsageTemplate(UsageWithNotes(notesMonitors))
	rootCmd.AddCommand(monitorsCmd)
}

func TestLogMonitors(args []string) {
	if !utils.IsTestMode() {
		return
	}
}

// EXISTING_CODE
// EXISTING_CODE
