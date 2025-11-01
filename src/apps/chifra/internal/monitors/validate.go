// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitorsPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/validate"
)

func (opts *MonitorsOptions) validateMonitors() error {
	chain := opts.Globals.Chain

	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if !config.IsChainConfigured(chain) {
		return validate.Usage("chain {0} is not properly configured.", chain)
	}

	if len(opts.Addrs) > 0 && (opts.List || opts.Count) {
		return validate.Usage("Do not provide addresses with {0} or {1}.", "--list", "--count")
	}

	if opts.List {
		// All other options are ignored

	} else {
		// Count dominates if present
		if !opts.Count {
			// We validate some of the simpler curd commands here and the rest in HandleCrud
			if opts.Undelete {
				if opts.Delete || opts.Remove {
					return validate.Usage("The --undelete option may not be used with --delete or --remove.")
				}
			}

			if !opts.Clean && len(opts.Addrs) == 0 {
				return validate.Usage("You must provide at least one Ethereum address for this command.")
			}

			if !opts.Clean && !opts.Delete && !opts.Undelete && !opts.Remove && !opts.Globals.Decache {
				return validate.Usage("Please provide either --clean or one of the CRUD commands.")
			}

			if !opts.Globals.IsApiMode() && !opts.Clean {
				if len(opts.Globals.File) > 0 {
					// Do nothing
				} else {
					err := validate.ValidateAtLeastOneNonSentinel(opts.Addrs)
					if err != nil {
						return err
					}
				}
			}
		}
	}

	return opts.Globals.Validate()
}
