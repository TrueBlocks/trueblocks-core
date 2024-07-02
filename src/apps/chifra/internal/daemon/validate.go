// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package daemonPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *DaemonOptions) validateDaemon() error {
	chain := opts.Globals.Chain

	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if !config.IsChainConfigured(chain) {
		return validate.Usage("chain {0} is not properly configured.", chain)
	}

	if opts.Globals.IsApiMode() {
		return validate.Usage("The {0} option is not available{1}.", "daemon", " in api mode")
	}

	if opts.Grpc {
		return validate.Usage("The {0} option is deprecated. There is no replacement.", "--grpc")
	}

	// validate.ValidateEnum("scrape", opts.Scrape, "[off|blooms|index]")
	// validate.ValidateEnum("api", opts.Api, "[off|on]")
	opts.Api = "on"

	if len(opts.Scrape) > 0 {
		return validate.Usage("The {0} option is currenlty not available. Use {1} instead.", "--scrape", "chifra scrape")
	}

	if opts.Monitor {
		return validate.Usage("The {0} option is currenlty not available. Use {1} instead.", "--monitor", "chifra monitors --watch")
	}

	return opts.Globals.Validate()
}
