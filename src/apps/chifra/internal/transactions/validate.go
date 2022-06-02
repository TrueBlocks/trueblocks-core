// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package transactionsPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *TransactionsOptions) ValidateTransactions() error {
	opts.TestLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if len(opts.Globals.File) > 0 {
		// Do nothing
	} else {
		if len(opts.Transactions) == 0 {
			return validate.Usage("Please supply one or more transaction identifiers.")
		}

		if len(opts.Reconcile) > 0 {
			if opts.Cache || opts.Trace || opts.Articulate {
				return validate.Usage("Do not use other options with the --reconcile option.")
			}
			if !validate.IsValidAddress(opts.Reconcile) {
				return validate.Usage("Invalid reconcilation address {0}.", opts.Reconcile)
			}
		}

		if opts.Trace && !rpcClient.IsTracingNode(opts.Globals.Chain) {
			return validate.Usage("Tracing is required for this program to work properly.")
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

	return opts.Globals.ValidateGlobals()
}
