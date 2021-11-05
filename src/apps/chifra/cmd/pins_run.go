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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/pins"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/spf13/cobra"
)

var errCustomFolderMissing = `Attempt to create customized indexPath (%s) failed.
Please create the folder or adjust the setting by editing $CONFIG/trueBlocks.toml.
`

func validatePinsArgs(cmd *cobra.Command, args []string) error {

	if PinsOpts.list && PinsOpts.init {
		return validate.Usage("Please choose only a single option.")
	}

	if !PinsOpts.list && !PinsOpts.init {
		return validate.Usage("You must choose at least one of --list or --init.")
	}

	if PinsOpts.all && !PinsOpts.init {
		return validate.Usage("Use the --all option only with the --init options.")
	}

	if PinsOpts.init_all {
		return validate.Deprecated("--init_all", "--init --all")
	}

	if PinsOpts.freshen {
		return validate.Deprecated("--freshen", "--init")
	}

	if PinsOpts.remote {
		return validate.Deprecated("--remote", "")
	}

	if PinsOpts.share {
		return validate.Usage("The --share option is not yet implemented")
	}

	// if (share) {
	//     string_q res := doCommand("which ipfs");
	//     if (res.empty()) {
	//         return usage("Could not find ipfs in your $PATH. You must install ipfs for the --share command to work.");
	// 	}
	// }
	// if (share) {
	//     ostringstream os;
	//     os << "ipfs add -Q --pin \"" << bloomFn + "\"";
	//     string_q newHash = doCommand(os.str());
	//     LOG_INFO(cGreen, "Re-pinning ", pin.fileName, cOff, " ==> ", newHash, " ",
	//          (pin.bloomHash == newHash ? greenCheck : redX));
	// }

	err := validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}

	utils.TestLogBool("list", PinsOpts.list)
	utils.TestLogBool("init", PinsOpts.init)
	utils.TestLogBool("all", PinsOpts.all)
	utils.TestLogBool("share", PinsOpts.share)
	utils.TestLogBool("remote", PinsOpts.remote)
	// LOG_TEST("sleep", sleep, (sleep == .25))

	return nil
}

func runPins(cmd *cobra.Command, args []string) {
	// This only happens in API mode when there's been an error. Here, we print the error
	if len(validate.Errors) > 0 {
		output.PrintJson(&output.JsonFormatted{})
		return
	}

	err := pinlib.EstablishDirectories()
	if err != nil {
		if err, ok := err.(*pinlib.ErrCustomizedPath); ok {
			fmt.Printf(errCustomFolderMissing, err.GetIndexPath())
			return
		}
		logger.Fatal(err)
	}

	if PinsOpts.list {
		pins.PrintManifestHeader()
		pins.HandleList()
		return
	}

	if PinsOpts.init {
		pins.PrintManifestHeader()
		pins.HandleInit(PinsOpts.all)
		return
	}

	if PinsOpts.sleep != .25 {
		logger.Fatal("Not implemented")
	}

	if PinsOpts.share {
		logger.Fatal("Not implemented")
	}
}
