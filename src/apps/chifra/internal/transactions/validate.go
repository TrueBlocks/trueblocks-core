// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package transactionsPkg

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *TransactionsOptions) validateTransactions() error {
	chain := opts.Globals.Chain

	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	key := config.GetRootConfig().Keys["trueblocks"].License
	if len(opts.AccountFor) > 0 && !strings.Contains(key, "+accounting") {
		return validate.Usage("The {0} option requires a license key. Please contact us in our discord.", "--accounting")
	}

	if len(opts.Flow) > 0 {
		if !opts.Uniq {
			return validate.Usage("The {0} option is only available with the {1} option", "--flow", "--uniq")
		}
		err := validate.ValidateEnum("flow", opts.Flow, "[from|to]")
		if err != nil {
			return err
		}
	}

	if len(opts.Globals.File) > 0 {
		// Do nothing
	} else {
		if !opts.Logs && (len(opts.Emitter) > 0 || len(opts.Topic) > 0) {
			return validate.Usage("The {0} option are only available with the {1} option.", "--emitter and --topic", "--log")
		} else if opts.Logs {
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
		}

		if len(opts.Transactions) == 0 {
			return validate.Usage("Please supply one or more transaction identifiers.")
		}

		if len(opts.AccountFor) > 0 {
			if opts.Uniq {
				return validate.Usage("The {0} option is not available{1}.", "--uniq", " with the --account_for option")
			}
			if !base.IsValidAddress(opts.AccountFor) {
				return validate.Usage("Invalid reconcilation address {0}.", opts.AccountFor)
			}
		}

		if opts.Traces {
			if !opts.Conn.IsNodeTracing() {
				return validate.Usage("{0} requires tracing, err: {1}", "chifra transactions --traces", rpc.ErrTraceBlockMissing)
			}
			if opts.Uniq {
				return validate.Usage("The {0} option is not available{1}.", "--uniq", " with the --traces option")
			}
		}

		if !validate.CanArticulate(opts.Articulate) {
			return validate.Usage("The {0} option requires an Etherscan API key.", "--articulate")
		}
	}

	err := validate.ValidateIdentifiers(
		chain,
		opts.Transactions,
		validate.ValidTransId,
		-1,
		&opts.TransactionIds,
	)
	if err != nil {
		if invalidLiteral, ok := err.(*validate.InvalidIdentifierLiteralError); ok {
			return invalidLiteral
		}
		return err
	}

	return opts.Globals.Validate()
}
