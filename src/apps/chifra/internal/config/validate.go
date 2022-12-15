// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package configPkg

import (
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *ConfigOptions) validateConfig() error {
	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if opts.Depth != uint64(0xffffffffffffffff) && opts.Depth > 3 {
		return validate.Usage("The {0} option ({1}) must {2}.", "--depth", strconv.FormatUint(opts.Depth, 10), "be less than four (4)")
	}

	err := validate.ValidateEnumSlice("--types", opts.Types, "[blocks|txs|traces|slurps|all]")
	if err != nil {
		return err
	}

	err = validate.ValidateEnumSlice("module", opts.Module, "[index|monitors|names|abis|caches|some|all]")
	if err != nil {
		return err
	}

	err = validate.ValidateEnumSlice("modes", opts.Modes, "[show|edit]")
	if err != nil {
		return err
	}

	return opts.Globals.Validate()
}
