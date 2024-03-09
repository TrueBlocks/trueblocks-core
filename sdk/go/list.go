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
	list "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type ListOptions struct {
	Addrs       []string     `json:"addrs,omitempty"`
	Count       bool         `json:"count,omitempty"`
	NoZero      bool         `json:"noZero,omitempty"`
	Bounds      bool         `json:"bounds,omitempty"`
	Unripe      bool         `json:"unripe,omitempty"`
	Silent      bool         `json:"silent,omitempty"`
	FirstRecord uint64       `json:"firstRecord,omitempty"`
	MaxRecords  uint64       `json:"maxRecords,omitempty"`
	Reversed    bool         `json:"reversed,omitempty"`
	FirstBlock  base.Blknum  `json:"firstBlock,omitempty"`
	LastBlock   base.Blknum  `json:"lastBlock,omitempty"`
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// String implements the stringer interface
func (opts *ListOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// List implements the chifra list command for the SDK.
func (opts *ListOptions) List(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	for _, v := range opts.Addrs {
		values.Add("addrs", v)
	}
	if opts.Count {
		values.Set("count", "true")
	}
	if opts.NoZero {
		values.Set("noZero", "true")
	}
	if opts.Bounds {
		values.Set("bounds", "true")
	}
	if opts.Unripe {
		values.Set("unripe", "true")
	}
	if opts.Silent {
		values.Set("silent", "true")
	}
	if opts.FirstRecord != 0 {
		values.Set("firstRecord", fmt.Sprint(opts.FirstRecord))
	}
	if opts.MaxRecords != 0 {
		values.Set("maxRecords", fmt.Sprint(opts.MaxRecords))
	}
	if opts.Reversed {
		values.Set("reversed", "true")
	}
	// if opts.Publisher != "" {
	// 	values.Set("publisher", opts.Publisher)
	// }
	if opts.FirstBlock > 0 {
		values.Set("firstBlock", fmt.Sprint(opts.FirstBlock))
	}
	if opts.LastBlock > 0 {
		values.Set("lastBlock", fmt.Sprint(opts.LastBlock))
	}
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return list.List(w, values)
}

// GetListOptions returns a filled-in options instance given a string array of arguments.
func GetListOptions(args []string) (*ListOptions, error) {
	var opts ListOptions
	if err := assignValuesFromArgs(&opts, &opts.Globals, args); err != nil {
		return nil, err
	}
	return &opts, nil
}

// No enums

// EXISTING_CODE
// EXISTING_CODE

