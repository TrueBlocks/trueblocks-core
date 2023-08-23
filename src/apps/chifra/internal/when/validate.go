// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *WhenOptions) validateWhen() error {
	chain := opts.Globals.Chain

	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if opts.Timestamps {
		if opts.List {
			return validate.Usage("Please choose only one of {0}.", "--timestamps or --list")
		}

		if opts.Deep && !opts.Check {
			return validate.Usage("The {0} option is only available with the {1} option.", "--deep", "--timestamps --check")
		}

		if opts.Repair && len(opts.Blocks) == 0 {
			return validate.Usage("The {0} option requires at least one block identifier.", "--repair")
		}

	} else {
		if opts.Check {
			return validate.Usage("The {0} option is only available with the {1} option.", "--check", "--timestamps")

		}

		if opts.Deep {
			return validate.Usage("The {0} option is only available with the {1} option.", "--deep", "--timestamps --check")
		}

		if opts.Count {
			return validate.Usage("The {0} option is only available with the {1} option.", "--count", "--timestamps")
		}

		if opts.Update {
			return validate.Usage("The {0} option is only available with the {1} option.", "--update", "--timestamps")
		}

		if opts.Truncate != utils.NOPOS {
			return validate.Usage("The {0} option is only available with the {1} option.", "--truncate", "--timestamps")
		}

		if opts.Repair {
			return validate.Usage("The {0} option is only available with the {1} option.", "--repair", "--timestamps")
		}
	}

	if len(opts.Blocks) == 0 {
		// If no identifier, then must be either --list or --timestamps
		if !opts.List && !opts.Timestamps {
			return validate.Usage("Please supply one or more block identifiers or one or more dates.")
		}

	} else {
		if opts.List && opts.Timestamps {
			// Cannot have both --list and --timestamps
			return validate.Usage("Please use either {0} or {1}.", "--list", "--timestamps")

		} else if opts.List {
			// Cannot have both block identifiers and --list
			return validate.Usage("Please supply either {0} or the {1} option.", "block identifiers", "--list")

		}
	}

	if opts.Globals.TestMode && opts.Timestamps && !opts.Check && !opts.Count {
		return validate.Usage("--timestamp option not tested in testMode")
	}

	err := validate.ValidateIdentifiers(
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

	return opts.Globals.Validate()
}
