// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tokensPkg

import (
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *TokensOptions) validateTokens() error {
	chain := opts.Globals.Chain

	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if !config.IsChainConfigured(chain) {
		return validate.Usage("chain {0} is not properly configured.", chain)
	}

	err := validate.ValidateEnumSlice("--parts", opts.Parts, "[name|symbol|decimals|totalSupply|version|some|all]")
	if err != nil {
		return err
	}

	if opts.Changes {
		return validate.Usage("The {0} option is not yet implemented.", "--changes")
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
				if addr != base.FAKE_ETH_ADDRESS.Hex() {
					err := opts.Conn.IsContractAtLatest(base.HexToAddress(addr))
					if err != nil {
						if errors.Is(err, rpc.ErrNotAContract) {
							return validate.Usage("The value {0} is not a token contract.", addr)
						}
						return err
					}
				}
			}
		} else {
			// the first is assumed to be a smart contract, the rest can be either non-existent, another smart contract or an EOA
			addr := opts.Addrs[0]
			if addr != base.FAKE_ETH_ADDRESS.Hex() {
				err := opts.Conn.IsContractAtLatest(base.HexToAddress(addr))
				if err != nil {
					if errors.Is(err, rpc.ErrNotAContract) {
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
			chain,
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

		latest := opts.Conn.GetLatestBlockNumber()
		if bounds.First < (latest-250) && !opts.Conn.IsNodeArchive() {
			return validate.Usage("The {0} requires {1}.", "query for historical state", "an archive node")
		}
	}

	return opts.Globals.Validate()
}
