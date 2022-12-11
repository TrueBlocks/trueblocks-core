// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package nodePkg

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"

func (opts *NodeOptions) validateNode() error {
	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	err := validate.ValidateEnum("scrape", opts.Scrape, "[off|blooms|full-index]")
	if err != nil {
		return err
	}

	err = validate.ValidateEnum("api", opts.Api, "[off|on]")
	if err != nil {
		return err
	}

	if !opts.Monitor && len(opts.Scrape) == 0 && len(opts.Api) == 0 {
		return validate.Usage("At least one of --scrape, --monitors, or --api must be present.")
	}

	return opts.Globals.Validate()
}
