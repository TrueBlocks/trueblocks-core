// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package daemonPkg

import (
	"strings"

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

	if len(opts.Scrape) > 0 {
		return validate.Usage("The {0} option is currenlty not available. Use {1} instead.", "--scrape", "chifra scrape")
	}

	if opts.Monitor {
		return validate.Usage("The {0} option is currenlty not available. Use {1} instead.", "--monitor", "chifra monitors --watch")
	}

	opts.Api = "on"

	if len(opts.Port) > 0 && !strings.Contains(opts.Port, ":") {
		opts.Port = ":" + opts.Port
	}

	return opts.Globals.Validate()
}
