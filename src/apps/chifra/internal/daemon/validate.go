// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package daemonPkg

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *DaemonOptions) validateDaemon() error {
	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if opts.Globals.IsApiMode() {
		return validate.Usage("The {0} option is not available{1}.", "daemon", " in api mode")
	}

	err := validate.ValidateEnum("scrape", opts.Scrape, "[off|blooms|index]")
	if err != nil {
		return err
	}
	if len(opts.Scrape) > 0 && opts.Scrape != "index" {
		return validate.Usage("Only the {0} option is available for {1}.", "index", "--scrape")
	}
	if opts.Monitor {
		return validate.Usage("The {0} option is currenlty not available. Use {1} instead.", "--monitor", "chifra monitors --watch")
	}

	err = validate.ValidateEnum("api", opts.Api, "[off|on]")
	if err != nil {
		return err
	}

	if !opts.Monitor && len(opts.Scrape) == 0 && len(opts.Api) == 0 {
		return validate.Usage("At least one of --scrape, --monitors, or --api must be present.")
	}

	if len(opts.Port) > 0 && !strings.Contains(opts.Port, ":") {
		return validate.Usage("The {0} option ({1}) must {2}.", "--port", opts.Port, "start with ':'")
	}

	return opts.Globals.Validate()
}
