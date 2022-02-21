// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package slurpPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *SlurpOptions) ValidateSlurp() error {
	opts.TestLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	err := validate.ValidateAtLeastOneAddr(opts.Addrs)
	if err != nil {
		return err
	}

	err = validate.ValidateEnumSlice("--types", opts.Types, "[ext|int|token|nfts|miner|uncles|all]")
	if err != nil {
		return err
	}

	// BOGUS
	if opts.Globals.Chain != "mainnet" {
		return validate.Usage("The {0} command is currently available only on the {1} chain.", "slurp", "mainnet")
	}

	return opts.Globals.ValidateGlobals()
}
