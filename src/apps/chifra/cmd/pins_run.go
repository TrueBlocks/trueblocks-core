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
package cmd

import (
	"fmt"

	"github.com/spf13/cobra"
)

func validatePinsArgs(cmd *cobra.Command, args []string) error {
	list := PinsOpts.list
	init := PinsOpts.init
	freshen := PinsOpts.freshen
	remote := PinsOpts.remote
	all := PinsOpts.all
	init_all := PinsOpts.init_all

	if list && (init || freshen) {
		return usage("Please choose only a single option.")
	}

	if !list && !init && !freshen {
		return usage("You must choose at least one of --list, --init, or --freshen.")
	}

	if remote && !list {
		return usage("The --remote option is only available with the --list option")
	}

	if remote {
		return usage("The --remote option is not yet implemented")
	}

	if all && !init {
		return usage("Use the --all option only with the --init or --freshen options.")
	}

	if init_all {
		return usage("Flag --init_all has been deprecated, use --init --all instead")
	}

	// if (share) {
	//     string_q res := doCommand("which ipfs");
	//     if (res.empty()) {
	//         return usage("Could not find ipfs in your $PATH. You must install ipfs for the --share command to work.");
	// 	}
	// }

	err := validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}

	return nil
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
	if PinsOpts.sleep != .25 {
		options += " --sleep " + fmt.Sprintf("%.1f", PinsOpts.sleep)
	}
	if PinsOpts.share {
		options += " --share"
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn("pinMan", options, arguments)
}
