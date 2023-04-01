// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package statusPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *StatusOptions) validateStatus() error {
	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	err := validate.ValidateEnumSlice("--types", opts.Types, "[blocks|txs|traces|slurps|all]")
	if err != nil {
		return err
	}

	err = validate.ValidateEnum("mode", opts.Mode, "[index|monitors|names|abis|caches|some|all]")
	if err != nil {
		return err
	}

	return opts.Globals.Validate()
}
