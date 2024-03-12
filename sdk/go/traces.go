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

	traces "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type TracesOptions struct {
	TransactionIds []string `json:"transactions,omitempty"`
	Articulate     bool     `json:"articulate,omitempty"`
	Filter         string   `json:"filter,omitempty"`
	Count          bool     `json:"count,omitempty"`
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// String implements the stringer interface
func (opts *TracesOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Traces implements the chifra traces command for the SDK.
func (opts *TracesOptions) Traces(w io.Writer) error {
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
	if opts.Filter != "" {
		values.Set("filter", opts.Filter)
	}
	if opts.Count {
		values.Set("count", "true")
	}
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return traces.Traces(w, values)
}

// GetTracesOptions returns a filled-in options instance given a string array of arguments.
func GetTracesOptions(args []string) (*TracesOptions, error) {
	var opts TracesOptions
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
