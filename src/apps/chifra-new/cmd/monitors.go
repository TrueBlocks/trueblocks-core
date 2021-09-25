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

import (
	"os"
	"strconv"

	"github.com/spf13/cobra"
)

type monitorsOptionsType struct {
	delete bool
	remove bool
}

var MonitorsOpts monitorsOptionsType

// monitorsCmd represents the monitors command
var monitorsCmd = &cobra.Command{
	Use: `monitors [flags] <address> [address...]

Arguments:
  addrs - one or more addresses (0x...) to list (required)`,
	Short: "add, remove, clean, and list address monitors",
	Long: `Purpose:
  Add, remove, clean, and list address monitors.`,
	Run: runMonitors,
}

func init() {
	monitorsCmd.Flags().SortFlags = false
	monitorsCmd.PersistentFlags().SortFlags = false
	monitorsCmd.SetOut(os.Stderr)

	monitorsCmd.Flags().BoolVarP(&MonitorsOpts.delete, "delete", "", false, "delete a previously created monitor (or undelete if already deleted)")
	monitorsCmd.Flags().BoolVarP(&MonitorsOpts.remove, "remove", "", false, "remove a previously deleted monitor")

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
	for _, arg := range args {
		options += " " + arg
	}
	PassItOn("/Users/jrush/.local/bin/chifra/acctExport --appearances", options, strconv.FormatUint(0, 10))
}
