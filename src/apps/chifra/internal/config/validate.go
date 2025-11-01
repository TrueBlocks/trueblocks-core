// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package configPkg

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/validate"
)

func (opts *ConfigOptions) validateConfig() error {
	chain := opts.Globals.Chain

	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if !config.IsChainConfigured(chain) {
		return validate.Usage("chain {0} is not properly configured.", chain)
	}

	if opts.Paths {
		if len(opts.Mode) > 0 && opts.Mode != "<empty>" {
			return validate.Usage("You must supply either {0} or {1}.", "a mode", "--paths")
		}
	} else {
		if err := validate.ValidateEnum("modes", opts.Mode, "[show|edit]"); err != nil {
			return err
		}
	}

	if !opts.Globals.TestMode && opts.Mode == "edit" && os.Getenv("EDITOR") == "" {
		return validate.Usage("You must set the EDITOR environment variable to use the 'edit' mode.")
	}

	// if len(opts.Mode) == 0 || !opts.Paths {
	// 	return validate.Usage("You must supply either {0} or {1}.", "a mode", "--paths")
	// }

	return opts.Globals.Validate()
}
