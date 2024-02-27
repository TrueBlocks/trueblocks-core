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
	"fmt"
	"io"
	"net/url"

	traces "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type TracesOptions struct {
	TransactionIds []string
	Articulate     bool
	Filter         string
	Count          bool
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// Traces implements the chifra traces command for the SDK.
func (opts *TracesOptions) Traces(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	for _, v := range opts.TransactionIds {
		values.Add("transactions", v)
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

// GetTracesOptions returns an options instance given a string array of arguments.
func GetTracesOptions(args []string) (*TracesOptions, error) {
	var opts TracesOptions

	for _, arg := range args {
		// EXISTING_CODE
		fmt.Println("arg: ", arg)
		// EXISTING_CODE
	}

	return &opts, nil
}

// EXISTING_CODE
// EXISTING_CODE
