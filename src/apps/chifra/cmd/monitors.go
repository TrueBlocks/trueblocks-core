package cmd

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
 * Parts of this file were generated with makeClass --gocmds.
 */

import (
	"errors"
	"os"

	"github.com/spf13/cobra"
)

// monitorsCmd represents the monitors command
var monitorsCmd = &cobra.Command{
	Use:   usageMonitors,
	Short: shortMonitors,
	Long:  longMonitors,
	Run:   runMonitors,
	Args:  ValidatePositionals(validateMonitorsArgs),
}

var usageMonitors = `monitors [flags] <address> [address...]

Arguments:
  addrs - one or more addresses (0x...) to list (required)`

var shortMonitors = "add, remove, clean, and list address monitors"

var longMonitors = `Purpose:
  Add, remove, clean, and list address monitors.`

var notesMonitors = ``

type monitorsOptionsType struct {
	delete bool
	remove bool
}

var MonitorsOpts monitorsOptionsType

func init() {
	monitorsCmd.SetOut(os.Stderr)

	monitorsCmd.Flags().SortFlags = false
	monitorsCmd.PersistentFlags().SortFlags = false
	monitorsCmd.Flags().BoolVarP(&MonitorsOpts.delete, "delete", "", false, "delete a previously created monitor (or undelete if already deleted)")
	monitorsCmd.Flags().BoolVarP(&MonitorsOpts.remove, "remove", "", false, "remove a previously deleted monitor")
	monitorsCmd.Flags().SortFlags = false
	monitorsCmd.PersistentFlags().SortFlags = false

	monitorsCmd.SetUsageTemplate(HelpWithNotes(notesMonitors))
	rootCmd.AddCommand(monitorsCmd)
}

func runMonitors(cmd *cobra.Command, args []string) {
	options := ""
	if MonitorsOpts.delete {
		options += " --delete"
	}
	if MonitorsOpts.remove {
		options += " --remove"
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn(GetCommandPath("acctExport --appearances"), options, arguments)
}

func validateMonitorsArgs(cmd *cobra.Command, args []string) error {
	if len(args) > 0 && args[0] == "12" {
		return ErrFunc(cmd, errors.New("Invalid argument "+args[0]))
	}
	return nil
}
