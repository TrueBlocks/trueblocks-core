// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package statusPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *StatusOptions) validateStatus() error {
	chain := opts.Globals.Chain

	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if !config.IsChainConfigured(chain) {
		return validate.Usage("chain {0} is not properly configured.", chain)
	}

	options := `[index|blooms|blocks|transactions|traces|logs|statements|results|state|tokens|monitors|names|abis|slurps|staging|unripe|maps|some|all]`
	err := validate.ValidateEnumSlice("mode", opts.Modes, options)
	if err != nil {
		return err
	}

	if opts.MaxRecords == 0 {
		return validate.Usage("{0} must be greater than zero", "--max_records")
	}

	return opts.Globals.Validate()
}
