// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tokensPkg

import (
	"errors"

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

	err = validate.ValidateIdentifiers(
		opts.Globals.Chain,
		opts.Blocks,
		validate.ValidBlockIdWithRangeAndDate,
		1,
		&opts.BlockIds,
	)
	if err != nil {
		if invalidLiteral, ok := err.(*validate.InvalidIdentifierLiteralError); ok {
			return invalidLiteral
		}

		if errors.Is(err, validate.ErrTooManyRanges) {
			return validate.Usage("Specify only a single block range at a time.")
		}

		return err
	}

	if len(opts.Addrs2) == 0 {
		return validate.Usage("You must specifiy at least one address")
	} else {
		if opts.ByAcct {
			if len(opts.Addrs2) < 2 {
				return validate.Usage("You must specifiy at least two addresses")
			}

			// all but the last is assumed to be a token
			for _, addr := range opts.Addrs2[:len(opts.Addrs2)-1] {
				ok, err := validate.IsSmartContract(opts.Globals.Chain, addr)
				if err != nil {
					return err
				}
				if !ok {
					return validate.Usage("The value {0} is not a token contract.", addr)
				}
			}
		} else {
			// the first is assumed to be a smart contract, the rest can be either token or no
			addr := opts.Addrs2[0]
			ok, err := validate.IsSmartContract(opts.Globals.Chain, addr)
			if err != nil {
				return err
			}
			if !ok {
				return validate.Usage("The value {0} is not a token contract.", addr)
			}
		}
	}

	err = validate.ValidateAddresses(opts.Addrs2)
	if err != nil {
		return err
	}

	return opts.Globals.ValidateGlobals()
}
