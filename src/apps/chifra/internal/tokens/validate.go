// return usage("Use either --parts or provide at least one token and one other account.");
// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tokensPkg

import (
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *TokensOptions) validateTokens() error {
	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	err := validate.ValidateEnumSlice("--parts", opts.Parts, "[name|symbol|decimals|totalSupply|version|all]")
	if err != nil {
		return err
	}

	if opts.Changes {
		return validate.Usage("The {0} is not yet implemented.", "--changes")
	}

	if err != nil {
		if invalidLiteral, ok := err.(*validate.InvalidIdentifierLiteralError); ok {
			return invalidLiteral
		}

		if errors.Is(err, validate.ErrTooManyRanges) {
			return validate.Usage("Specify only a single block range at a time.")
		}

		return err
	}

	if len(opts.Addrs) == 0 {
		return validate.Usage("You must specify at least two address")

	} else {
		if opts.ByAcct {
			if len(opts.Addrs) < 2 {
				return validate.Usage("You must specify at least two addresses")
			}

			// all but the last is assumed to be a token
			for _, addr := range opts.Addrs[:len(opts.Addrs)-1] {
				err := rpcClient.IsContractAt(opts.Globals.Chain, base.HexToAddress(addr), nil)
				if err != nil {
					if errors.Is(err, rpcClient.ErrNotAContract) {
						return validate.Usage("The value {0} is not a token contract.", addr)
					}
					return err
				}
			}
		} else {
			// the first is assumed to be a smart contract, the rest can be either non-existant, another smart contract or an EOA
			addr := opts.Addrs[0]
			err := rpcClient.IsContractAt(opts.Globals.Chain, base.HexToAddress(addr), nil)
			if err != nil {
				if err != nil {
					if errors.Is(err, rpcClient.ErrNotAContract) {
						return validate.Usage("The value {0} is not a token contract.", addr)
					}
					return err
				}
			}
		}
	}

	err = validate.ValidateAddresses(opts.Addrs)
	if err != nil {
		return err
	}

	// Blocks are optional, but if they are present, they must be valid
	if len(opts.Blocks) > 0 {
		bounds, err := validate.ValidateIdentifiersWithBounds(
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

		latest := rpcClient.GetLatestBlockNumber(opts.Globals.Chain)
		// TODO: Should be configurable
		if bounds.First < (latest-250) && !rpcClient.IsNodeArchive(opts.Globals.Chain) {
			return validate.Usage("The {0} requires {1}.", "query for historical state", "an archive node")
		}
	}

	return opts.Globals.Validate()
}
