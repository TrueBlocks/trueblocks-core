// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package blocksPkg

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
		valid, err := validate.IsValidAddressE(emitter)
		if !valid {
			return err
		}
	}

	for _, topic := range opts.Topic {
		valid, err := validate.IsValidTopicE(topic)
		if !valid {
			return err
		}
	}

	if opts.ListCount == 0 {
		err := validate.ValidateIdentifiers(
			opts.Globals.Chain,
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

	if len(opts.Globals.File) > 0 {
		// Do nothing
	} else {
		// if opts.ListCount != 0 && len(opts.Blocks) != 0 {
		// 	fmt.Println(opts.ListCount)
		// 	fmt.Println(len(opts.Blocks))
		// 	return validate.Usage("Supply either the --list_count option or block identifiers, not both.")
		// } else
		if opts.List > 0 {
			if opts.ListCount == 0 {
				return validate.Usage("You must supply a non-zero value for the --list_count option with --list.")
			}

		} else {
			if len(opts.Blocks) == 0 && opts.ListCount == 0 {
				return validate.Usage("Please supply one or more block identifiers or the --list_count option.")
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
