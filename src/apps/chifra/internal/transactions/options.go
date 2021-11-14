package transactionsPkg

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
 * The file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

type TransactionsOptions struct {
	Transactions []string
	Articulate   bool
	Trace        bool
	Uniq         bool
	Reconcile    string
	Cache        bool
	Globals      globals.GlobalOptionsType
}

func (opts *TransactionsOptions) TestLog() {
	logger.TestLog(len(opts.Transactions) > 0, "Transactions: ", opts.Transactions)
	logger.TestLog(opts.Articulate, "Articulate: ", opts.Articulate)
	logger.TestLog(opts.Trace, "Trace: ", opts.Trace)
	logger.TestLog(opts.Uniq, "Uniq: ", opts.Uniq)
	logger.TestLog(len(opts.Reconcile) > 0, "Reconcile: ", opts.Reconcile)
	logger.TestLog(opts.Cache, "Cache: ", opts.Cache)
	opts.Globals.TestLog()
}

func FromRequest(w http.ResponseWriter, r *http.Request) *TransactionsOptions {
	opts := &TransactionsOptions{}
	for key, value := range r.URL.Query() {
		switch key {
		case "transactions":
			opts.Transactions = append(opts.Transactions, value...)
		case "articulate":
			opts.Articulate = true
		case "trace":
			opts.Trace = true
		case "uniq":
			opts.Uniq = true
		case "reconcile":
			opts.Reconcile = value[0]
		case "cache":
			opts.Cache = true
		}
	}
	opts.Globals = *globals.FromRequest(w, r)

	return opts
}
