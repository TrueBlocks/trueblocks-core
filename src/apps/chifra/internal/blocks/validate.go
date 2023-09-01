// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package blocksPkg

import (
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *BlocksOptions) validateBlocks() error {
	chain := opts.Globals.Chain

	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	for _, emitter := range opts.Emitter {
		valid, err := base.IsValidAddressE(emitter)
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
		if opts.List > 0 {
			return validate.Usage("You must supply a non-zero value for the {0} option with {1}.", "--list_count", "--list")
		}
	} else {
		if opts.List == 0 {
			return validate.Usage("You must supply a non-zero value for the {0} option with {1}.", "--list", "--list_count")
		}
	}

	if len(opts.Flow) > 0 {
		if !opts.Uniq {
			return validate.Usage("The {0} option is only available with the {1} option", "--flow", "--uniq")
		}
		err := validate.ValidateEnum("flow", opts.Flow, "[from|to|reward]")
		if err != nil {
			return err
		}
	}

	if len(opts.Globals.File) > 0 {
		// Do nothing
	} else {
		if opts.List == 0 {
			if len(opts.Blocks) == 0 && opts.ListCount == 0 {
				return validate.Usage("Please supply either a block identifier or the --list_count option.")
			}
			if !opts.Logs && (len(opts.Emitter) > 0 || len(opts.Topic) > 0) {
				return validate.Usage("The {0} option are only available with the {1} option.", "--emitter and --topic", "--log")
			}
			if opts.Traces && opts.Hashes {
				return validate.Usage("The {0} option is not available{1}.", "--traces", " with the --hashes option")
			}
			if !validate.CanArticulate(opts.Articulate) {
				return validate.Usage("The {0} option requires an Etherscan API key.", "--articulate")
			}
			if opts.Articulate && !opts.Logs {
				return validate.Usage("The {0} option is only available with the {1} option.", "--articulate", "--logs")
			}
			if opts.Uniq && !opts.Count {
				if opts.Traces {
					return validate.Usage("The {0} option is not available{1}.", "--traces", " with the --uniq option")
				}
				if opts.Uncles {
					return validate.Usage("The {0} option is not available{1}.", "--uncles", " with the --uniq option")
				}
				if opts.Logs {
					return validate.Usage("The {0} option is not available{1}.", "--logs", " with the --uniq option")
				}
			}
			if opts.BigRange != 500 && !opts.Logs {
				return validate.Usage("The {0} option is only available with the {1} option.", "--big_range", "--logs")
			}

			if opts.Traces && !opts.Conn.IsNodeTracing() {
				return validate.Usage("{0} requires tracing, err: {1}", "chifra blocks --traces", rpc.ErrTraceBlockMissing)
			}
		}

		if opts.Articulate {
			if opts.Uncles {
				return validate.Usage("The {0} option is not available{1}.", "--articulate", " with the --uncles option")
			}
			if opts.List != 0 {
				return validate.Usage("The {0} option is not available{1}.", "--articulate", " with the --list option")
			}
		}

		// We cannot cache uncles because they are identical to the cannonical blocks of the same number and would be incorrectly retreived.
		if opts.Globals.Cache && opts.Uncles {
			return validate.Usage("The {0} option is currently not available{1}.", "--cache", " with the --uncles option")
		}
	}

	return opts.Globals.Validate()
}
