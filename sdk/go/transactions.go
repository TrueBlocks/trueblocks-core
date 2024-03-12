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
	"encoding/json"
	"io"
	"net/url"
	"strings"

	transactions "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type TransactionsOptions struct {
	TransactionIds []string         `json:"transactions,omitempty"`
	Articulate     bool             `json:"articulate,omitempty"`
	Traces         bool             `json:"traces,omitempty"`
	Uniq           bool             `json:"uniq,omitempty"`
	Flow           TransactionsFlow `json:"flow,omitempty"`
	Logs           bool             `json:"logs,omitempty"`
	Emitter        []string         `json:"emitter,omitempty"`
	Topic          []string         `json:"topic,omitempty"`
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// String implements the stringer interface
func (opts *TransactionsOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Transactions implements the chifra transactions command for the SDK.
func (opts *TransactionsOptions) Transactions(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	for _, v := range opts.TransactionIds {
		items := strings.Split(v, " ")
		for _, item := range items {
			values.Add("transactions", item)
		}
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
		items := strings.Split(v, " ")
		for _, item := range items {
			values.Add("emitter", item)
		}
	}
	for _, v := range opts.Topic {
		items := strings.Split(v, " ")
		for _, item := range items {
			values.Add("topic", item)
		}
	}
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return transactions.Transactions(w, values)
}

// GetTransactionsOptions returns a filled-in options instance given a string array of arguments.
func GetTransactionsOptions(args []string) (*TransactionsOptions, error) {
	var opts TransactionsOptions
	if err := assignValuesFromArgs(args, nil, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	// EXISTING_CODE
	// EXISTING_CODE

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
