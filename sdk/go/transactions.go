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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
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
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// Transactions implements the chifra transactions command for the SDK.
func (opts *TransactionsOptions) Transactions(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	for _, v := range opts.TransactionIds {
		values.Add("transactions", v)
	}
	if opts.Articulate {
		values.Set("articulate", "true")
	}
	if opts.Traces {
		values.Set("traces", "true")
	}
	if opts.Uniq {
		values.Set("uniq", "true")
	}
	if opts.Flow != NoTF {
		values.Set("flow", opts.Flow.String())
	}
	if opts.Logs {
		values.Set("logs", "true")
	}
	for _, v := range opts.Emitter {
		values.Add("emitter", v)
	}
	for _, v := range opts.Topic {
		values.Add("topic", v)
	}
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return transactions.Transactions(w, values)
}

// GetTransactionsOptions returns an options instance given a string array of arguments.
func GetTransactionsOptions(args []string) (*TransactionsOptions, error) {
	var opts TransactionsOptions
	err := assignValuesFromArgs(&opts, &opts.Globals, args)
	logger.Info("Args:", args)
	logger.Info("Opts:", opts.String())
	if err != nil {
		return nil, err
	}

	return &opts, nil
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
