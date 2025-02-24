// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package blocksPkg

import (
	"errors"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *BlocksOptions) validateBlocks() error {
	chain := opts.Globals.Chain

	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if !config.IsChainConfigured(chain) {
		return validate.Usage("chain {0} is not properly configured.", chain)
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

	if which, tooMany := opts.tooMany(); tooMany {
		return validate.Usage("Please choose only a single mode ({0}, etc.)", strings.Join(which, ", "))
	}

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
		if len(opts.Blocks) == 0 {
			return validate.Usage("Please supply at least one {0}.", "block identifier")
		}
		if !opts.Logs && (len(opts.Emitter) > 0 || len(opts.Topic) > 0) {
			return validate.Usage("The {0} option are only available with the {1} option.", "--emitter and --topic", "--log")
		}
		if opts.Traces && opts.Hashes {
			return validate.Usage("The {0} option is not available{1}.", "--traces", " with the --hashes option")
		}
		if !validate.HasArticulationKey(opts.Articulate) {
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

		if opts.Traces {
			err, ok := opts.Conn.IsNodeTracing()
			if !ok {
				return validate.Usage("{0} requires tracing, err: {1}", "chifra blocks --traces", err.Error())
			}
		}

		if opts.Articulate {
			if opts.Uncles {
				return validate.Usage("The {0} option is not available{1}.", "--articulate", " with the --uncles option")
			}
		}

		// We cannot cache uncles because they are identical to the canonical blocks of the same number and would be incorrectly retreived.
		if opts.Globals.Cache && opts.Uncles {
			return validate.Usage("The {0} option is currently not available{1}.", "--cache", " with the --uncles option")
		}
		if opts.Globals.Decache && opts.Uncles {
			return validate.Usage("The {0} option is currently not available{1}.", "--decache", " with the --uncles option")
		}
	}

	return opts.Globals.Validate()
}

func (opts *BlocksOptions) tooMany() ([]string, bool) {
	which := []string{}
	cnt := 0
	if opts.Uncles {
		which = append(which, "--uncles")
		cnt++
	}
	if opts.Traces {
		which = append(which, "--traces")
		cnt++
	}
	if opts.Uniq {
		which = append(which, "--uniq")
		cnt++
	}
	if opts.Logs {
		which = append(which, "--logs")
		cnt++
	}
	if opts.Withdrawals {
		which = append(which, "--withdrawals")
		cnt++
	}

	if len(which) > 2 {
		which = which[:2]
	}

	return which, !opts.Count && cnt > 1
}
