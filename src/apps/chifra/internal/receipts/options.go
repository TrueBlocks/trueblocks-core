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
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

import (
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

type ReceiptsOptions struct {
	Transactions []string
	Articulate   bool
	Globals      globals.GlobalOptionsType
	BadFlag      error
}

func (opts *ReceiptsOptions) TestLog() {
	logger.TestLog(len(opts.Transactions) > 0, "Transactions: ", opts.Transactions)
	logger.TestLog(opts.Articulate, "Articulate: ", opts.Articulate)
	opts.Globals.TestLog()
}

func (opts *ReceiptsOptions) ToCmdLine() string {
	options := ""
	if opts.Articulate {
		options += " --articulate"
	}
	options += " " + strings.Join(opts.Transactions, " ")
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

func FromRequest(w http.ResponseWriter, r *http.Request) *ReceiptsOptions {
	opts := &ReceiptsOptions{}
	for key, value := range r.URL.Query() {
		switch key {
		case "transactions":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Transactions = append(opts.Transactions, s...)
			}
		case "articulate":
			opts.Articulate = true
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "receipts")
				return opts
			}
		}
	}
	opts.Globals = *globals.FromRequest(w, r)

	return opts
}

var Options ReceiptsOptions

func ReceiptsFinishParse(args []string) *ReceiptsOptions {
	Options.Transactions = args
	// EXISTING_CODE
	// EXISTING_CODE
	return &Options
}
