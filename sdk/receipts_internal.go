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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	receipts "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type receiptsOptionsInternal struct {
	TransactionIds []string `json:"transactions,omitempty"`
	Articulate     bool     `json:"articulate,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *receiptsOptionsInternal) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// ReceiptsBytes implements the chifra receipts command for the SDK.
func (opts *receiptsOptionsInternal) ReceiptsBytes(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		return fmt.Errorf("error converting receipts struct to URL values: %v", err)
	}

	return receipts.Receipts(w, values)
}

// receiptsParseFunc handles special cases such as structs and enums (if any).
func receiptsParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	_, ok := target.(*receiptsOptionsInternal)
	if !ok {
		return false, fmt.Errorf("parseFunc(receipts): target is not of correct type")
	}

	// No enums
	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// GetReceiptsOptions returns a filled-in options instance given a string array of arguments.
func GetReceiptsOptions(args []string) (*receiptsOptionsInternal, error) {
	var opts receiptsOptionsInternal
	if err := assignValuesFromArgs(args, receiptsParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

type receiptsGeneric interface {
	types.Receipt
}

func queryReceipts[T receiptsGeneric](opts *receiptsOptionsInternal) ([]T, *types.MetaData, error) {
	// EXISTING_CODE
	// EXISTING_CODE

	buffer := bytes.Buffer{}
	if err := opts.ReceiptsBytes(&buffer); err != nil {
		return nil, nil, err
	}

	str := buffer.String()
	// EXISTING_CODE
	str = convert("inputs", str)
	str = convert("outputs", str)
	// EXISTING_CODE

	var result Result[T]
	if err := json.Unmarshal([]byte(str), &result); err != nil {
		return nil, nil, err
	} else {
		return result.Data, &result.Meta, nil
	}
}

// toInternal converts the SDK options to the internal options format.
func (opts *ReceiptsOptions) toInternal() *receiptsOptionsInternal {
	return &receiptsOptionsInternal{
		TransactionIds: opts.TransactionIds,
		Articulate:     opts.Articulate,
		Globals:        opts.Globals,
	}
}

// EXISTING_CODE
// EXISTING_CODE
