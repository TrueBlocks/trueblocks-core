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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *BlocksOptions) ValidateBlocks() error {
	opts.TestLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	for _, emitter := range opts.Emitter {
		valid, err := validate.IsValidAddress(emitter)
		if !valid {
			return err
		}
	}

	for _, topic := range opts.Topic {
		valid, err := validate.IsValidTopic(topic)
		if !valid {
			return err
		}
	}

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

	if len(opts.Globals.File) > 0 {
		// Do nothing
	} else {
		if opts.List > 0 {
			// Do nothing
		} else {
			// We check ListCount for the default value here, but what we really want is to know
			// if it was supplied as a parameter
			if len(opts.Blocks) == 0 && opts.ListCount == 20 {
				return validate.Usage("Please supply one or more block identifiers.")
			}
			if !opts.Logs && (len(opts.Emitter) > 0 || len(opts.Topic) > 0) {
				return validate.Usage("The {0} option are only available with the {1} option.", "--emitter and --topic", "--log")
			}
			if opts.Cache && opts.Uncles {
				return validate.Usage("The {0} option is not available{1}.", "--cache", " with the --uncles option")
			}
			if opts.Trace && opts.Hashes {
				return validate.Usage("The {0} option is not available{1}.", "--trace", " with the --hashes option")
			}
			if opts.Articulate && !opts.Logs {
				return validate.Usage("The {0} option is available only with {1}.", "--articulate", "the --logs option")
			}
			if opts.Uniq {
				if opts.Trace {
					return validate.Usage("The {0} option is not available{1}.", "--trace", " with the --uniq option")
				}
				if opts.Cache {
					return validate.Usage("The {0} option is not available{1}.", "--cache", " with the --uniq option")
				}
			}
			if opts.Apps {
				if opts.Trace {
					return validate.Usage("The {0} option is not available{1}.", "--trace", " with the --apps option")
				}
				if opts.Cache {
					return validate.Usage("The {0} option is not available{1}.", "--cache", " with the --apps option")
				}
			}
		}
	}

	return opts.Globals.ValidateGlobals()
}
