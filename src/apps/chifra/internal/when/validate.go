// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *WhenOptions) ValidateWhen() error {
	opts.TestLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if len(opts.Blocks) == 0 && (!opts.List && !opts.Timestamps) {
		return validate.Usage("Please supply one or more block identifiers or one or more dates.")
	}

	if len(opts.Blocks) > 0 && (opts.List || opts.Timestamps) {
		return validate.Usage("Please use the --list or --timestamp option or supply block identifiers, not both.")
	}

	if opts.List && opts.Timestamps {
		return validate.Usage("Please use the --list or --timestamp option, not both.")
	}

	if !opts.Timestamps {
		if opts.Fix || opts.Check || opts.Count {
			return validate.Usage("The {0} option are only available with the {1} option.", "--check, --fix, and --count", "--timestamps")
		}
	}

	err := validate.ValidateIdentifiers(opts.Blocks, validate.ValidBlockIdWithRangeAndDate, 1)
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
