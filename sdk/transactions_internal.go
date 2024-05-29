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
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	transactions "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type transactionsOptionsInternal struct {
	TransactionIds []string         `json:"transactions,omitempty"`
	Articulate     bool             `json:"articulate,omitempty"`
	Traces         bool             `json:"traces,omitempty"`
	Uniq           bool             `json:"uniq,omitempty"`
	Flow           TransactionsFlow `json:"flow,omitempty"`
	Logs           bool             `json:"logs,omitempty"`
	Emitter        []string         `json:"emitter,omitempty"`
	Topic          []string         `json:"topic,omitempty"`
	CacheTraces    bool             `json:"cacheTraces,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *transactionsOptionsInternal) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// TransactionsBytes implements the chifra transactions command for the SDK.
func (opts *transactionsOptionsInternal) TransactionsBytes(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		return fmt.Errorf("error converting transactions struct to URL values: %v", err)
	}

	return transactions.Transactions(w, values)
}

// transactionsParseFunc handles special cases such as structs and enums (if any).
func transactionsParseFunc(target any, key, value string) (bool, error) {
	var found bool
	opts, ok := target.(*transactionsOptionsInternal)
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
func GetTransactionsOptions(args []string) (*transactionsOptionsInternal, error) {
	var opts transactionsOptionsInternal
	if err := assignValuesFromArgs(args, transactionsParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

type transactionsGeneric interface {
	types.Transaction |
		types.Trace |
		types.Appearance |
		types.Log |
		base.Address
}

func queryTransactions[T transactionsGeneric](opts *transactionsOptionsInternal) ([]T, *types.MetaData, error) {
	// EXISTING_CODE
	// EXISTING_CODE

	buffer := bytes.Buffer{}
	if err := opts.TransactionsBytes(&buffer); err != nil {
		return nil, nil, err
	}

	str := buffer.String()
	// EXISTING_CODE
	str = convertObjectToArray("inputs", str)
	str = convertObjectToArray("outputs", str)
	// EXISTING_CODE

	var result Result[T]
	if err := json.Unmarshal([]byte(str), &result); err != nil {
		debugPrint(str, result, err)
		return nil, nil, err
	} else {
		return result.Data, &result.Meta, nil
	}
}

// toInternal converts the SDK options to the internal options format.
func (opts *TransactionsOptions) toInternal() *transactionsOptionsInternal {
	return &transactionsOptionsInternal{
		TransactionIds: opts.TransactionIds,
		Articulate:     opts.Articulate,
		Flow:           opts.Flow,
		Emitter:        opts.Emitter,
		Topic:          opts.Topic,
		CacheTraces:    opts.CacheTraces,
		Globals:        opts.Globals,
	}
}

// EXISTING_CODE
// EXISTING_CODE
