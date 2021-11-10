package pins

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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/root"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/spf13/cobra"
)

func Validate(cmd *cobra.Command, args []string) error {

	if Options.List && Options.Init {
		return validate.Usage("Please choose only a single option.")
	}

	if !Options.List && !Options.Init {
		return validate.Usage("You must choose at least one of --list or --init.")
	}

	if Options.All && !Options.Init {
		return validate.Usage("Use the --all option only with the --init options.")
	}

	if Options.Init_All {
		return validate.Deprecated("--init_all", "--init --all")
	}

	if Options.Freshen {
		return validate.Deprecated("--freshen", "--init")
	}

	if Options.Remote {
		return validate.Deprecated("--remote", "")
	}

	if Options.Share {
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

	err := root.ValidateGlobals(cmd, args)
	if err != nil {
		return err
	}

	utils.TestLogBool("list", Options.List)
	utils.TestLogBool("init", Options.Init)
	utils.TestLogBool("all", Options.All)
	utils.TestLogBool("share", Options.Share)
	utils.TestLogBool("remote", Options.Remote)
	// LOG_TEST("sleep", sleep, (sleep == .25))

	return nil
}
