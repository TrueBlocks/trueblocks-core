// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *ChunksOptions) ValidateChunks() error {
	opts.TestLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if len(opts.Mode) == 0 {
		return validate.Usage("Please choose at least one of {0}.", "[stats|pins|blooms|index|addresses|appearances]")
	}

	err := validate.ValidateEnum("mode", opts.Mode, "[stats|pins|blooms|index|addresses|appearances]")
	if err != nil {
		return err
	}

	if opts.Globals.LogLevel > 0 && opts.Mode == "addresses" && opts.Globals.Format == "json" {
		return validate.Usage("You may not use --format json and log_level > 0 in addresses mode")
	}

	if opts.Belongs {
		if opts.Mode != "addresses" && opts.Mode != "blooms" {
			return validate.Usage("The --belongs option is only available with either the addresses or blooms mode")
		}
		if len(opts.Addrs) == 0 {
			return validate.Usage("You must specifiy at least one address with the --belongs option")
		}
		if len(opts.Blocks) == 0 {
			return validate.Usage("You must specifiy at least one block identifier with the --belongs option")
		}
	}

	if len(opts.Addrs) > 0 && !opts.Belongs {
		return validate.Usage("You may only specify an address with the --belongs option")
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

	return opts.Globals.ValidateGlobals()
}
