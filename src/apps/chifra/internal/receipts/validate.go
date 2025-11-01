// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package receiptsPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/validate"
)

func (opts *ReceiptsOptions) validateReceipts() error {
	chain := opts.Globals.Chain

	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if !config.IsChainConfigured(chain) {
		return validate.Usage("chain {0} is not properly configured.", chain)
	}

	if len(opts.Globals.File) > 0 {
		// Do nothing
	} else {
		if len(opts.Transactions) == 0 {
			return validate.Usage("Please supply one or more transaction identifiers.")
		}
		if !validate.HasArticulationKey(opts.Articulate) {
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

	// for _, arg := range args {
	// 	valid, err := validateTxIdentifier(arg)
	// 	if !valid || err != nil {
	// 		return err
	// 	}
	// }

	return opts.Globals.Validate()
}
