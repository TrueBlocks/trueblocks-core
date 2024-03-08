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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	initPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type InitOptions struct {
	All        bool
	DryRun     bool
	FirstBlock base.Blknum
	Sleep      float64
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// String implements the stringer interface
func (opts *InitOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Init implements the chifra init command for the SDK.
func (opts *InitOptions) Init(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	if opts.All {
		values.Set("all", "true")
	}
	if opts.DryRun {
		values.Set("dry_run", "true")
	}
	if opts.FirstBlock != 0 {
		values.Set("first_block", fmt.Sprintf("%d", opts.FirstBlock))
	}
	if opts.Sleep != 0 {
		values.Set("sleep", fmt.Sprint(opts.Sleep))
	}
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return initPkg.Init(w, values)
}

// GetInitOptions returns a filled-in options instance given a string array of arguments.
func GetInitOptions(args []string) (*InitOptions, error) {
	var opts InitOptions
	if err := assignValuesFromArgs(&opts, &opts.Globals, args); err != nil {
		return nil, err
	}
	return &opts, nil
}

// No enums

// EXISTING_CODE
// EXISTING_CODE

