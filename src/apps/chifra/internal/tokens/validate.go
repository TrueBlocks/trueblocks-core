// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tokensPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *TokensOptions) ValidateTokens() error {
	opts.TestLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	err := validate.ValidateEnumSlice("--parts", opts.Parts, "[name|symbol|decimals|totalSupply|version|none|all]")
	if err != nil {
		return err
	}

	// special case for tokens which don't allow --dollars display
	if opts.Globals.Dollars {
		return validate.Usage("The {0} option is not available{1}.", "--dollars", " with this tool")
	}

	if len(opts.Addrs2) == 0 {
		return validate.Usage("You must specifiy at least one address")
	} else if len(opts.Addrs2) > 1 {
		if opts.ByAcct {
			// all but the last is assumed to be a token
			for _, addr := range opts.Addrs2[:len(opts.Addrs2)-1] {
				if !validate.IsSmartContract(addr) {
					return validate.Usage("The value {0} is not a token contract.", addr)
				}
			}
		} else {
			// all but the first are assumed to be a token
			for _, addr := range opts.Addrs2[1:] {
				if !validate.IsSmartContract(addr) {
					return validate.Usage("The value {0} is not a token contract.", addr)
				}
			}
		}
	}

	return opts.Globals.ValidateGlobals()
}
