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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/spf13/cobra"
)

func validateChunksArgs(cmd *cobra.Command, args []string) error {
	list := ChunksOpts.list
	check := ChunksOpts.check
	extract := false // ChunksOpts.extract
	blooms := false  // ChunkOpts.blooms
	// freshen := ChunksOpts.freshen
	// remote := ChunksOpts.remote
	// all := ChunksOpts.all
	// init_all := ChunksOpts.init_all

	if !list && !check && !extract {
		return validate.Usage("You must choose at least one of {0}.", "--list, --extract, or --check")
	}

	if (list && check) || (list && extract) || (check && extract) {
		return validate.Usage("Please choose just one of {0}.", "--list, --extract, or --check")
	}

	if blooms && !(extract || check) {
		return validate.Usage("The {0} option is only available with the {1} option.", "--bloom", "--extract or --check")
	}

	// if !list && !init && !freshen {
	// 	return validate.Usage("You must choose at least one of --list, --init, or --freshen.")
	// }

	// if remote && !list {
	// 	return validate.Usage("The --remote option is only available with the --list option")
	// }

	// if remote {
	// 	return validate.Usage("The --remote option is not yet implemented")
	// }

	// if all && !init {
	// 	return validate.Usage("Use the --all option only with the --init or --freshen options.")
	// }

	// if init_all {
	// 	return validate.Usage("Flag --init_all has been deprecated, use --init --all instead")
	// }

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

func runChunks(cmd *cobra.Command, args []string) {
	options := ""
	if ChunksOpts.list {
		options += " --list"
	}
	if ChunksOpts.check {
		options += " --check"
	}
	if ChunksOpts.extract {
		options += " --extract"
	}
	if ChunksOpts.blooms {
		options += " --blooms"
	}
	// if ChunksOpts.all {
	// 	options += " --all"
	// }
	// if ChunksOpts.sleep != .25 {
	// 	options += " --sleep " + fmt.Sprintf("%.1f", ChunksOpts.sleep)
	// }
	// if ChunksOpts.share {
	// 	options += " --share"
	// }
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn("chunkMan", options, arguments)
}
