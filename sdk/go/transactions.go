// Copyright 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package sdk

import (
	// EXISTING_CODE
	"io"
	"net/url"

	transactions "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type TransactionsOptions struct {
	TransactionIds []string
	Articulate     bool
	Traces         bool
	Uniq           bool
	Flow           TransactionsFlow
	Logs           bool
	Emitter        []string // allow for ENS names and addresses
	Topic          []string // topics are strings
	CacheTraces    bool
	Seed           bool
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// Transactions implements the chifra transactions command for the SDK.
func (opts *TransactionsOptions) Transactions(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	//   transactions - a space-separated list of one or more transaction identifiers (required)
	//   -a, --articulate        articulate the retrieved data if ABIs can be found
	//   -t, --traces            include the transaction's traces in the results
	//   -u, --uniq              display a list of uniq addresses found in the transaction
	//   -f, --flow string       for the uniq option only, export only from or to (including trace from or to)
	//                           One of [ from | to ]
	//   -l, --logs              display only the logs found in the transaction(s)
	//   -m, --emitter strings   for the --logs option only, filter logs to show only those logs emitted by the given address(es)
	//   -B, --topic strings     for the --logs option only, filter logs to show only those with this topic(s)
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return transactions.Transactions(w, values)
}

type TransactionsFlow int

const (
	NoTF TransactionsFlow = iota
	TFFrom
	TFTo
)

func (v TransactionsFlow) String() string {
	return []string{
		"notf",
		"from",
		"to",
	}[v]
}

// EXISTING_CODE
// EXISTING_CODE

