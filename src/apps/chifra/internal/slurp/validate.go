// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package slurpPkg

import (
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *SlurpOptions) validateSlurp() error {
	chain := opts.Globals.Chain

	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	err := validate.ValidateAtLeastOneAddr(opts.Addrs)
	if err != nil {
		return err
	}

	err = validate.ValidateEnumSlice("--types", opts.Types, "[ext|int|token|nfts|1155|miner|uncles|all]")
	if err != nil {
		return err
	}

	if chain != "mainnet" {
		return validate.Usage("The {0} command is currently available only on the {1} chain.", "slurp", "mainnet")
	}

	if opts.Sleep < .25 {
		return validate.Usage("The {0} option ({1}) must {2}.", "--sleep", fmt.Sprintf("%f", opts.Sleep), "be at least .25")
	}

	if opts.PerPage < 10 {
		return validate.Usage("The {0} option ({1}) must {2}.", "--per_page", fmt.Sprintf("%d", opts.PerPage), "be at least 10")
	} else if opts.PerPage > 10000 {
		return validate.Usage("The {0} option ({1}) must {2}.", "--per_page", fmt.Sprintf("%d", opts.PerPage), "be no more than 10,000")
	}

	err = validate.ValidateIdentifiers(
		chain,
		opts.Blocks,
		validate.ValidBlockIdWithRange,
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

	return opts.Globals.Validate()
}
