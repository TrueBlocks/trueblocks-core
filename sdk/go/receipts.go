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

	receipts "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type ReceiptsOptions struct {
	TransactionIds []string `json:"transactions,omitempty"`
	Articulate     bool     `json:"articulate,omitempty"`
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// String implements the stringer interface
func (opts *ReceiptsOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Receipts implements the chifra receipts command for the SDK.
func (opts *ReceiptsOptions) Receipts(w io.Writer) error {
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
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return receipts.Receipts(w, values)
}

// GetReceiptsOptions returns a filled-in options instance given a string array of arguments.
func GetReceiptsOptions(args []string) (*ReceiptsOptions, error) {
	var opts ReceiptsOptions
	if err := assignValuesFromArgs(args, nil, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return &opts, nil
}

// No enums

// EXISTING_CODE
// EXISTING_CODE
