// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package namesPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/validate"
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

	if opts.Count {
		if opts.Clean || len(opts.Autoname) > 0 || opts.anyCrud() {
			return validate.Usage("You may not use the {0} option when editing names.", "--count")
		}
		if len(opts.Terms) > 0 {
			return validate.Usage("The {0} option is not available{1}.", "--count", " with search terms")
		}
		/*
			Custom    bool                  `json:"custom,omitempty"`    // Include only custom named accounts in the search
			Prefund   bool                  `json:"prefund,omitempty"`   // Include prefund accounts in the search
			Regular   bool                  `json:"regular,omitempty"`   // Only available with --clean, cleans regular names database
			Tags      bool                  `json:"tags,omitempty"`      // Export the list of tags and subtags only
		*/
	}

	if len(opts.Terms) != 1 {
		if opts.Delete {
			return validate.Usage("The {0} option requires exactly one address.", "--delete")
		} else if opts.Undelete {
			return validate.Usage("The {0} option requires exactly one address.", "--undelete")
		} else if opts.Remove {
			return validate.Usage("The {0} option requires exactly one address.", "--remove")
		}
	}

	isDryRunnable := opts.Clean || len(opts.Autoname) > 0
	if opts.DryRun && !isDryRunnable {
		return validate.Usage("The {0} option is only available with the {1} options.", "--dry_run", "--clean or --autoname")
	}

	if opts.Tags {
		if opts.Addr {
			return validate.Usage("The {0} option is not available{1}.", "--tags", " with the --addr option")
		}
		if opts.Clean {
			return validate.Usage("The {0} option is not available{1}.", "--tags", " with the --clean option")
		}
		if opts.anyCrud() {
			return validate.Usage("The {0} option is not available{1}.", "--tags", " with the any CRUD option")
		}
	}

	if opts.Addr {
		if opts.Tags {
			return validate.Usage("The {0} option is not available{1}.", "--addr", " with the --tags option")
		}
		if opts.Clean {
			return validate.Usage("The {0} option is not available{1}.", "--addr", " with the --clean option")
		}
		if opts.anyCrud() {
			return validate.Usage("The {0} option is not available{1}.", "--addr", " with the any CRUD option")
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
			return validate.Usage("The {0} option is not available{1}.", "--regular", " with the --autoname option")
		}

		if !base.IsValidAddress(opts.Autoname) || opts.AutonameAddr.IsZero() {
			return validate.Usage("You must provide an address to the {0} option.", "--autoname")
		}
		if err := opts.Conn.IsContractAtLatest(opts.AutonameAddr); err != nil {
			if err == rpc.ErrNotAContract {
				return validate.Usage("The address provided to the {0} option is not a contract.", "--autoname")
			}
			// ignore this error... we'll catch it later
		}
	}

	return opts.Globals.Validate()
}
