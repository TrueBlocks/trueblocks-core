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
	"fmt"
	"os"

	"github.com/spf13/cobra"
)

// pinsCmd represents the pins command
var pinsCmd = &cobra.Command{
	Use:   usagePins,
	Short: shortPins,
	Long:  longPins,
	Run:   runPins,
	Args:  ValidatePositionals(validatePinsArgs),
}

var usagePins = `pins [flags]`

var shortPins = "manage pinned index of appearances and associated Bloom filters"

var longPins = `Purpose:
  Manage pinned index of appearances and associated Bloom filters.`

type pinsOptionsType struct {
	list        bool
	init        bool
	init_all    bool
	sleep       float64
	remote      bool
	pin_locally bool
}

var PinsOpts pinsOptionsType

func init() {
	pinsCmd.SetOut(os.Stderr)

	pinsCmd.Flags().SortFlags = false
	pinsCmd.PersistentFlags().SortFlags = false
	pinsCmd.Flags().BoolVarP(&PinsOpts.list, "list", "l", false, "list the index and Bloom filter hashes from local manifest or pinning service")
	pinsCmd.Flags().BoolVarP(&PinsOpts.init, "init", "i", false, "initialize local index by downloading Bloom filters from pinning service")
	pinsCmd.Flags().BoolVarP(&PinsOpts.init_all, "init_all", "n", false, "initialize local index by downloading both Bloom filters and index chunks")
	pinsCmd.Flags().Float64VarP(&PinsOpts.sleep, "sleep", "s", 0.0, "the number of seconds to sleep between requests during init (default .25) (hidden)")
	pinsCmd.Flags().BoolVarP(&PinsOpts.remote, "remote", "r", false, "applicable only to --list mode, recover the manifest from pinning service (hidden)")
	pinsCmd.Flags().BoolVarP(&PinsOpts.pin_locally, "pin_locally", "p", false, "pin all local files in the index to an IPFS store (requires IPFS)")
	pinsCmd.Flags().SortFlags = false
	pinsCmd.PersistentFlags().SortFlags = false

	PostNotes = ""
	rootCmd.AddCommand(pinsCmd)
}

func runPins(cmd *cobra.Command, args []string) {
	options := ""
	if PinsOpts.list {
		options += " --list"
	}
	if PinsOpts.init {
		options += " --init"
	}
	if PinsOpts.init_all {
		options += " --init_all"
	}
	if PinsOpts.sleep > 0.0 {
		options += " --sleep " + fmt.Sprintf("%.1f", PinsOpts.sleep)
	}
	if PinsOpts.remote {
		options += " --remote"
	}
	if PinsOpts.pin_locally {
		options += " --pin_locally"
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn("/Users/jrush/.local/bin/chifra/pinMan", options, arguments)
}

func validatePinsArgs(cmd *cobra.Command, args []string) error {
	if len(args) > 0 && args[0] == "12" {
		return ErrFunc(cmd, errors.New("Invalid argument "+args[0]))
	}
	return nil
}
