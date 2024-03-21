// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package namesPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *NamesOptions) validateNames() error {
	chain := opts.Globals.Chain

	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if !config.IsChainConfigured(chain) {
		return validate.Usage("chain {0} is not properly configured.", chain)
	}

	isDryRunnable := opts.Clean || len(opts.Autoname) > 0
	if opts.DryRun && !isDryRunnable {
		return validate.Usage("The {0} option is only available with the {1} options.", "--dry_run", "--clean or --autoname")
	}

	if opts.Tags {
		if opts.Addr {
			return validate.Usage("The {0} option is not available with the {1} option.", "--tags", "--addr")
		}
		if opts.Clean {
			return validate.Usage("The {0} option is not available with the {1} option.", "--tags", "--clean")
		}
		if opts.anyCrud() {
			return validate.Usage("The {0} option is not available with the {1} option.", "--tags", "any CRUD")
		}
	}

	if opts.Addr {
		if opts.Tags {
			return validate.Usage("The {0} option is not available with the {1} option.", "--addr", "--tag")
		}
		if opts.Clean {
			return validate.Usage("The {0} option is not available with the {1} option.", "--addr", "--clean")
		}
		if opts.anyCrud() {
			return validate.Usage("The {0} option is not available with the {1} option.", "--addr", "any CRUD")
		}
	}

	if opts.MatchCase && len(opts.Terms) == 0 {
		return validate.Usage("The {0} option requires at least one {1}.", "--match_case", "term")
	}

	if opts.Prefund {
		if opts.Clean || len(opts.Autoname) > 0 || opts.anyCrud() {
			return validate.Usage("You may not use the {0} option when editing names.", "--prefund")
		}
	}

	if len(opts.Autoname) > 0 {
		if opts.Regular {
			return validate.Usage("The {0} option is not available with the {1} option.", "--regular", "--autoname")
		}

		if !base.IsValidAddress(opts.Autoname) || opts.AutonameAddr.IsZero() {
			return validate.Usage("You must provide an address to the {0} option.", "--autoname")
		}
		if err := opts.Conn.IsContractAt(opts.AutonameAddr, nil); err != nil {
			if err == rpc.ErrNotAContract {
				return validate.Usage("The address provided to the {0} option is not a token contract.", "--autoname")
			}
			// ignore this error... we'll catch it later
		}
	}

	return opts.Globals.Validate()
}
