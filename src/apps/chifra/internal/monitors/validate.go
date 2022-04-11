// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitorsPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *MonitorsOptions) ValidateMonitors() error {
	opts.TestLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	// We validate some of the simpler curd commands here and the rest in HandleCrudCommands
	if opts.Undelete {
		if opts.Delete || opts.Remove {
			return validate.Usage("The --undelete option may not be used with --delete or --remove.")
		}
	}

	if !opts.Clean && len(opts.Addrs) == 0 {
		return validate.Usage("You must provide at least one Ethereum address for this command.")
	}

	if !opts.Clean && !opts.Delete && !opts.Undelete && !opts.Remove {
		return validate.Usage("Please provide either --clean or one of the CRUD commands.")
	}

	if !opts.Globals.ApiMode && !opts.Clean {
		if len(opts.Globals.File) > 0 {
			// Do nothing
		} else {
			err := validate.ValidateAtLeastOneAddr(opts.Addrs)
			if err != nil {
				return err
			}
		}
	}

	return opts.Globals.ValidateGlobals()
}
