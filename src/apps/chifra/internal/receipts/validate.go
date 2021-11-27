// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package receiptsPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *ReceiptsOptions) ValidateReceipts() error {
	opts.TestLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	// err := validate.ValidateIdentifiers(opts.Transactions, validate.ValidTransId, -1)
	// if err != nil {
	// 	if invalidLiteral, ok := err.(*validate.InvalidIdentifierLiteralError); ok {
	// 		return invalidLiteral
	// 	}
	// 	return err
	// }

	if len(opts.Globals.File) > 0 {
		// Do nothing
	} else {
		if len(opts.Transactions) == 0 {
			return validate.Usage("Please supply one or more transaction identifiers.")
		}
	}

	// for _, arg := range args {
	// 	valid, err := validateTxIdentifier(arg)
	// 	if !valid || err != nil {
	// 		return err
	// 	}
	// }

	return opts.Globals.ValidateGlobals()
}
