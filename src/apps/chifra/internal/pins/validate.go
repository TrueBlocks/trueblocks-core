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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"

	"github.com/spf13/cobra"
)

func Validate(cmd *cobra.Command, args []string) error {
	output.Format = Options.Globals.Format
	err := Options.ValidateOptionsPins()
	if err != nil {
		return err
	}
	return nil
}

func (opts *PinsOptionsType) ValidateOptionsPins() error {

	if opts.List && opts.Init {
		return validate.Usage("Please choose only one of {0}.", "--list or --init")
	}

	if !opts.List && !opts.Init {
		return validate.Usage("Please choose at least one of {0}.", "--list or --init")
	}

	if opts.All && !opts.Init {
		return validate.Usage("The {0} option is available only with {1}.", "--all", "--init")
	}

	if opts.InitAll {
		return validate.Deprecated("--init_all", "--init --all")
	}

	if opts.Freshen {
		return validate.Deprecated("--freshen", "--init")
	}

	if opts.Remote {
		return validate.Deprecated("--remote", "")
	}

	if opts.Share {
		return validate.Usage("The {0} option is not available{1}.", "--share", " (not implemented)")
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

	Options.TestLog()

	return root.ValidateGlobals2(&opts.Globals, []string{})
}
