// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package sdk

import (
	// EXISTING_CODE
	"encoding/json"
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	// EXISTING_CODE
)

type TransactionsOptions struct {
	TransactionIds []string         `json:"transactions,omitempty"`
	Articulate     bool             `json:"articulate,omitempty"`
	Flow           TransactionsFlow `json:"flow,omitempty"`
	Emitter        []string         `json:"emitter,omitempty"`
	Topic          []string         `json:"topic,omitempty"`
	CacheTraces    bool             `json:"cacheTraces,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *TransactionsOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Transactions implements the chifra transactions command.
func (opts *TransactionsOptions) Transactions() ([]types.Transaction, *types.MetaData, error) {
	in := opts.toInternal()
	return queryTransactions[types.Transaction](in)
}

// TransactionsTraces implements the chifra transactions --traces command.
func (opts *TransactionsOptions) TransactionsTraces() ([]types.Trace, *types.MetaData, error) {
	in := opts.toInternal()
	in.Traces = true
	return queryTransactions[types.Trace](in)
}

// TransactionsUniq implements the chifra transactions --uniq command.
func (opts *TransactionsOptions) TransactionsUniq() ([]types.Appearance, *types.MetaData, error) {
	in := opts.toInternal()
	in.Uniq = true
	return queryTransactions[types.Appearance](in)
}

// TransactionsLogs implements the chifra transactions --logs command.
func (opts *TransactionsOptions) TransactionsLogs() ([]types.Log, *types.MetaData, error) {
	in := opts.toInternal()
	in.Logs = true
	return queryTransactions[types.Log](in)
}

// TransactionsSeed implements the chifra transactions --logs command.
func (opts *TransactionsOptions) TransactionsSeed() ([]types.Log, *types.MetaData, error) {
	in := opts.toInternal()
	in.Seed = true
	return queryTransactions[types.Log](in)
}

type TransactionsFlow int

const (
	NoTF   TransactionsFlow = 0
	TFFrom                  = 1 << iota
	TFTo
)

func (v TransactionsFlow) String() string {
	switch v {
	case NoTF:
		return "none"
	}

	var m = map[TransactionsFlow]string{
		TFFrom: "from",
		TFTo:   "to",
	}

	var ret []string
	for _, val := range []TransactionsFlow{TFFrom, TFTo} {
		if v&val != 0 {
			ret = append(ret, m[val])
		}
	}

	return strings.Join(ret, ",")
}

func enumFromTransactionsFlow(values []string) (TransactionsFlow, error) {
	if len(values) == 0 {
		return NoTF, fmt.Errorf("no value provided for flow option")
	}

	var result TransactionsFlow
	for _, val := range values {
		switch val {
		case "from":
			result |= TFFrom
		case "to":
			result |= TFTo
		default:
			return NoTF, fmt.Errorf("unknown flow: %s", val)
		}
	}

	return result, nil
}

// EXISTING_CODE
// EXISTING_CODE
