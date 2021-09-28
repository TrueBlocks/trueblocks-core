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
 * Parts of this file were generated with makeClass --gocmds. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

import (
	// EXISTING_CODE
	"fmt"
	"os"

	"github.com/spf13/cobra"
	// EXISTING_CODE
)

// pinsCmd represents the pins command
var pinsCmd = &cobra.Command{
	Use:   usagePins,
	Short: shortPins,
	Long:  longPins,
	Run:   runPins,
	Args:  validatePinsArgs,
}

var usagePins = `pins [flags]`

var shortPins = "manage pinned index of appearances and associated Bloom filters"

var longPins = `Purpose:
  Manage pinned index of appearances and associated Bloom filters.`

var notesPins = `
Notes:
  - One of --list, --init, or --freshen is required.
  - the --share option only works if the IPFS executable is in your path.`

type pinsOptionsType struct {
	list     bool
	init     bool
	freshen  bool
	remote   bool
	all      bool
	sleep    float64
	share    bool
	init_all bool
}

var PinsOpts pinsOptionsType

func init() {
	pinsCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE
	pinsCmd.Flags().SortFlags = false
	pinsCmd.PersistentFlags().SortFlags = false
	pinsCmd.Flags().BoolVarP(&PinsOpts.list, "list", "l", false, "list the index and Bloom filter hashes from local manifest or pinning service")
	pinsCmd.Flags().BoolVarP(&PinsOpts.init, "init", "i", false, "initialize index of appearances by downloading Bloom filters from pinning service")
	pinsCmd.Flags().BoolVarP(&PinsOpts.freshen, "freshen", "f", false, "freshen index of appearances using the same mode from most recent --init")
	pinsCmd.Flags().BoolVarP(&PinsOpts.remote, "remote", "r", false, "for --list mode only, recover the manifest from pinning service (hidden)")
	pinsCmd.Flags().BoolVarP(&PinsOpts.all, "all", "a", false, "for --init and --freshen modes only, download full index chunks as well as Bloom filter")
	pinsCmd.Flags().Float64VarP(&PinsOpts.sleep, "sleep", "s", 0.0, "the number of seconds to sleep between requests during download (default .25) (hidden)")
	pinsCmd.Flags().BoolVarP(&PinsOpts.share, "share", "S", false, "pin downloaded files to your local IPFS store, that is, share them (requires IPFS)")
	pinsCmd.Flags().BoolVarP(&PinsOpts.init_all, "init_all", "n", false, "use --init --all instead (hidden)")
	if IsTestMode() == false {
		pinsCmd.Flags().MarkHidden("remote")
		pinsCmd.Flags().MarkHidden("sleep")
		pinsCmd.Flags().MarkHidden("init_all")
	}
	pinsCmd.Flags().SortFlags = false
	pinsCmd.PersistentFlags().SortFlags = false
	// EXISTING_CODE
	// EXISTING_CODE

	pinsCmd.SetUsageTemplate(HelpWithNotes(notesPins))
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
	if PinsOpts.freshen {
		options += " --freshen"
	}
	if PinsOpts.remote {
		options += " --remote"
	}
	if PinsOpts.all {
		options += " --all"
	}
	if PinsOpts.sleep > 0.0 {
		options += " --sleep " + fmt.Sprintf("%.1f", PinsOpts.sleep)
	}
	if PinsOpts.share {
		options += " --share"
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	// EXISTING_CODE
	// EXISTING_CODE
	PassItOn(GetCommandPath("pinMan"), options, arguments)
}

// EXISTING_CODE
// EXISTING_CODE

func validatePinsArgs(cmd *cobra.Command, args []string) error {
	var err error
	// EXISTING_CODE
	if !PinsOpts.list && !PinsOpts.init && !PinsOpts.freshen {
		return makeError("You must choose at least one of {0}, {1}, or {2}", "--list", "--init", "--freshen")
	}
	// refuse to run deprecated commands
	if PinsOpts.init_all {
		return makeError("Flag --init_all has been deprecated, use --init --all instead")
	}
	// EXISTING_CODE
	// validate global arguments
	err = validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}
	return nil
}
