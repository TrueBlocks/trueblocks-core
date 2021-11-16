package blocksPkg

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

import (
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *BlocksOptions) ValidateBlocks() error {
	opts.TestLog()

	validationErr := validate.ValidateIdentifiers(
		opts.Blocks,
		validate.ValidBlockIdWithRange,
		1,
	)

	if validationErr != nil {
		if invalidLiteral, ok := validationErr.(*validate.InvalidIdentifierLiteralError); ok {
			return invalidLiteral
		}

		if errors.Is(validationErr, validate.ErrTooManyRanges) {
			return validate.Usage("Specify only a single block range at a time.")
		}

		return validationErr
	}

	if len(opts.Globals.File) > 0 {
		// Do nothing
	} else {
		if opts.List > 0 {
			// Do nothing
		} else {
			if opts.Cache && opts.Uniq {
				return validate.Usage("The {0} option is not available{1}.", "--cache", " with the --uniq option")
			}
			if len(opts.Blocks) == 0 {
				return validate.Usage("Please supply one or more block identifiers.")
			}
			if !opts.Logs && (len(opts.Emitter) > 0 || len(opts.Topic) > 0) {
				return validate.Usage("The {0} option are only available with the {1} option.", "--emitter and --topic", "--log")
			}
		}
	}

	return globals.ValidateGlobals(&opts.Globals)
}
