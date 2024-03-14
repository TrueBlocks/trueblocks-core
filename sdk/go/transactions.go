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
	"fmt"
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
	CacheTraces bool `json:"cacheTraces,omitempty"`
	// Seed           bool             `json:"seed,omitempty"`
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
	if opts.CacheTraces {
		values.Set("cacheTraces", "true")
	}
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return transactions.Transactions(w, values)
}

// transactionsParseFunc handles specail cases such as structs and enums (if any).
func transactionsParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	// EXISTING_CODE
	opts, ok := target.(*TransactionsOptions)
	if !ok {
		return false, fmt.Errorf("parseFunc(transactions): target is not of correct type")
	}

	switch key {
	case "flow":
		var err error
		values := strings.Split(value, ",")
		if opts.Flow, err = enumsFromStrsTransactions(values); err != nil {
			return false, err
		} else {
			found = true
		}
	}

	// EXISTING_CODE
	return found, nil
}

// GetTransactionsOptions returns a filled-in options instance given a string array of arguments.
func GetTransactionsOptions(args []string) (*TransactionsOptions, error) {
	var opts TransactionsOptions
	if err := assignValuesFromArgs(args, transactionsParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return &opts, nil
}

type TransactionsFlow int

const (
	NoTF TransactionsFlow = 0
	TFFrom = 1 << iota
	TFTo
)

func (v TransactionsFlow) String() string {
	switch v {
	case NoTF:
		return "none"
	}

	var m = map[TransactionsFlow]string{
		TFFrom: "from",
		TFTo: "to",
	}

	var ret []string
	for _, val := range []TransactionsFlow{TFFrom, TFTo} {
		if v&val != 0 {
			ret = append(ret, m[val])
		}
	}

	return strings.Join(ret, ",")
}

// EXISTING_CODE
func enumsFromStrsTransactions(values []string) (TransactionsFlow, error) {
	if len(values) == 0 {
		return NoTF, fmt.Errorf("no value provided for flow option")
	}

	var result TransactionsFlow
	for _, val := range values {
		switch val {
		case "from":
			result |= BFFrom
		case "to":
			result |= BFTo
		case "reward":
			result |= BFReward
		default:
			return NoTF, fmt.Errorf("unknown flow: %s", val)
		}
	}

	return result, nil
}

// EXISTING_CODE
