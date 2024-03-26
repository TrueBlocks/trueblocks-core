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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	receipts "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type ReceiptsOptions struct {
	TransactionIds []string `json:"transactions,omitempty"`
	Articulate     bool     `json:"articulate,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *ReceiptsOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Receipts implements the chifra receipts command for the SDK.
func (opts *ReceiptsOptions) Receipts(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		log.Fatalf("Error converting receipts struct to URL values: %v", err)
	}

	return receipts.Receipts(w, values)
}

// receiptsParseFunc handles specail cases such as structs and enums (if any).
func receiptsParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	_, ok := target.(*ReceiptsOptions)
	if !ok {
		return false, fmt.Errorf("parseFunc(receipts): target is not of correct type")
	}

	// No enums
	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// GetReceiptsOptions returns a filled-in options instance given a string array of arguments.
func GetReceiptsOptions(args []string) (*ReceiptsOptions, error) {
	var opts ReceiptsOptions
	if err := assignValuesFromArgs(args, receiptsParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

type receiptsResult struct {
	Data []types.SimpleReceipt `json:"data"`
	Meta rpc.MetaData          `json:"meta"`
}

func (opts *ReceiptsOptions) Query() ([]types.SimpleReceipt, *rpc.MetaData, error) {
	receiptsBuf := bytes.Buffer{}
	if err := opts.Receipts(&receiptsBuf); err != nil {
		logger.Fatal(err)
	}

	var receipts receiptsResult
	if err := json.Unmarshal(receiptsBuf.Bytes(), &receipts); err != nil {
		return nil, nil, err
	} else {
		return receipts.Data, &receipts.Meta, nil
	}
}

// No enums
// EXISTING_CODE
// EXISTING_CODE
