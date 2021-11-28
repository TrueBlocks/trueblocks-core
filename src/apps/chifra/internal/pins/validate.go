// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package pinsPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *PinsOptions) ValidatePins() error {
	opts.TestLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

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
	//         return usa ge("Could not find ipfs in your $PATH. You must install ipfs for the --share command to work.");
	// 	}
	// }
	// if (share) {
	//     ostringstream os;
	//     os << "ipfs add -Q --pin \"" << bloomFn + "\"";
	//     string_q newHash = doCommand(os.str());
	//     LOG_INFO(cGreen, "Re-pinning ", pin.fileName, cOff, " ==> ", newHash, " ",
	//          (pin.bloomHash == newHash ? greenCheck : redX));
	// }

	return opts.Globals.ValidateGlobals()
}
