package statePkg

import (
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/

func (opts *StateOptions) ValidateState() error {
	opts.TestLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	for _, part := range opts.Parts {
		err := validate.ValidateEnum("--parts", part, "[none|some|all|balance|nonce|code|storage|deployed|accttype]")
		if err != nil {
			return err
		}
	}

	if len(opts.Globals.File) > 0 {
		// do nothing for now

	} else {
		if len(opts.Call) > 0 {
			if len(opts.Parts) > 0 {
				return validate.Usage("The {0} option is not available{1}.", "--parts", " with the --call option")
			}

			if opts.Changes {
				return validate.Usage("The {0} option is not available{1}.", "--changes", " with the --call option")
			}

			if opts.NoZero {
				return validate.Usage("The {0} option is not available{1}.", "--no_zero", " with the --call option")
			}

			for _, addr := range opts.Addrs {
				ok, _ := validate.IsValidAddress(addr)
				if ok {
					return validate.Usage("The {0} option is not available{1}.", "--call", " when an address is present")
				}
			}

		} else {

			if len(opts.ProxyFor) > 0 {
				return validate.Usage("The --proxy_for option is only available with the --call option.")
			}

			err := validate.ValidateAtLeastOneAddr(opts.Addrs)
			if err != nil {
				return err
			}

		}
	}

	// Blocks are available for both commands
	if len(opts.Blocks) > 0 {
		err := validate.ValidateIdentifiers(
			opts.Blocks,
			validate.ValidBlockIdWithRange,
			1,
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
	}

	return opts.Globals.ValidateGlobals()
}
