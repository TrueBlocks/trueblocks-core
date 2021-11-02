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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/pinman"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/spf13/cobra"
)

func validatePinsArgs(cmd *cobra.Command, args []string) error {
	list := PinsOpts.list
	init := PinsOpts.init
	freshen := PinsOpts.freshen
	remote := PinsOpts.remote
	all := PinsOpts.all
	init_all := PinsOpts.init_all
	share := PinsOpts.share

	if list && (init || freshen) {
		return validate.Usage("Please choose only a single option.")
	}

	if !list && !init && !freshen {
		return validate.Usage("You must choose at least one of --list, --init, or --freshen.")
	}

	if remote && !list {
		return validate.Usage("The --remote option is only available with the --list option")
	}

	if remote {
		return validate.Usage("The --remote option is not yet implemented")
	}

	if all && !init {
		return validate.Usage("Use the --all option only with the --init or --freshen options.")
	}

	if init_all {
		return validate.Usage("Flag --init_all has been deprecated, use --init --all instead")
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

	utils.TestLogBool("list", list)
	utils.TestLogBool("init", init)
	utils.TestLogBool("freshen", freshen)
	utils.TestLogBool("all", all)
	utils.TestLogBool("share", share)
	utils.TestLogBool("remote", remote)
	// LOG_TEST("sleep", sleep, (sleep == .25))

	return nil
}

var errCustomFolderMissing = `Attempt to create customized indexPath (%s) failed.
Please create the folder or adjust the setting by editing $CONFIG/trueBlocks.toml.
`

func runPins(cmd *cobra.Command, args []string) {
	if PinsOpts.list {
		pinman.PrintManifestHeader()
		pinman.HandleList(RootOpts.fmt)
		return
	}

	if PinsOpts.init || PinsOpts.freshen {
		pinman.PrintManifestHeader()
		err := pinlib.EstablishDirectories()
		if err != nil {
			if err, ok := err.(*pinlib.ErrCustomizedPath); ok {
				fmt.Printf(errCustomFolderMissing, err.GetIndexPath())
				return
			}
			logger.Fatal(err)
		}

		pinman.HandleInit(PinsOpts.all)
		return
	}

	if PinsOpts.remote {
		logger.Fatal("Not implemented")
	}

	if PinsOpts.sleep != .25 {
		logger.Fatal("Not implemented")
	}

	if PinsOpts.share {
		logger.Fatal("Not implemented")
	}
}
