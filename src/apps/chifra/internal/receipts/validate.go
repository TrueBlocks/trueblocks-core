package receiptsPkg

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
