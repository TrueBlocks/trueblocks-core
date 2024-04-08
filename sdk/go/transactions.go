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
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"log"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
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
	CacheTraces    bool             `json:"cacheTraces,omitempty"`
	Seed           bool             `json:"seed,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *TransactionsOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// TransactionsBytes implements the chifra transactions command for the SDK.
func (opts *TransactionsOptions) TransactionsBytes(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		log.Fatalf("Error converting transactions struct to URL values: %v", err)
	}

	return transactions.Transactions(w, values)
}

// transactionsParseFunc handles special cases such as structs and enums (if any).
func transactionsParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	opts, ok := target.(*TransactionsOptions)
	if !ok {
		return false, fmt.Errorf("parseFunc(transactions): target is not of correct type")
	}

	if key == "flow" {
		var err error
		values := strings.Split(value, ",")
		if opts.Flow, err = enumFromTransactionsFlow(values); err != nil {
			return false, err
		} else {
			found = true
		}
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// GetTransactionsOptions returns a filled-in options instance given a string array of arguments.
func GetTransactionsOptions(args []string) (*TransactionsOptions, error) {
	var opts TransactionsOptions
	if err := assignValuesFromArgs(args, transactionsParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

type transactionsGeneric interface {
	types.SimpleTransaction |
		types.SimpleTrace |
		types.SimpleAppearance |
		types.SimpleLog
}

func queryTransactions[T transactionsGeneric](opts *TransactionsOptions) ([]T, *rpc.MetaData, error) {
	buffer := bytes.Buffer{}
	if err := opts.TransactionsBytes(&buffer); err != nil {
		logger.Fatal(err)
	}

	var result Result[T]
	if err := json.Unmarshal(buffer.Bytes(), &result); err != nil {
		return nil, nil, err
	} else {
		return result.Data, &result.Meta, nil
	}
}

// Transactions implements the chifra transactions command.
func (opts *TransactionsOptions) Transactions() ([]types.SimpleTransaction, *rpc.MetaData, error) {
	return queryTransactions[types.SimpleTransaction](opts)
}

// TransactionsTraces implements the chifra transactions --traces command.
func (opts *TransactionsOptions) TransactionsTraces() ([]types.SimpleTrace, *rpc.MetaData, error) {
	return queryTransactions[types.SimpleTrace](opts)
}

// TransactionsUniq implements the chifra transactions --uniq command.
func (opts *TransactionsOptions) TransactionsUniq() ([]types.SimpleAppearance, *rpc.MetaData, error) {
	return queryTransactions[types.SimpleAppearance](opts)
}

// TransactionsLogs implements the chifra transactions --logs command.
func (opts *TransactionsOptions) TransactionsLogs() ([]types.SimpleLog, *rpc.MetaData, error) {
	return queryTransactions[types.SimpleLog](opts)
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
