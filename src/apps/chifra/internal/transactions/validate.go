// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package transactionsPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *TransactionsOptions) validateTransactions() error {
	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
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
				return validate.Usage("The {0} option is not available with the {1} option", "--uniq", "--account_for")
			}
			if !base.IsValidAddress(opts.AccountFor) {
				return validate.Usage("Invalid reconcilation address {0}.", opts.AccountFor)
			}
		}

		if opts.Traces && !rpcClient.IsNodeTracing(opts.Globals.TestMode, opts.Globals.Chain) {
			return validate.Usage("Tracing is required for this program to work properly.")
		}

		if !validate.CanArticulate(opts.Articulate) {
			return validate.Usage("The {0} option requires an Etherscan API key.", "--articulate")
		}
	}

	err := validate.ValidateIdentifiers(
		opts.Globals.Chain,
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
