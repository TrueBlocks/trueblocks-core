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

	when "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type WhenOptions struct {
	BlockIds   []string `arg:"blocks" json:"blocks,omitempty"`
	List       bool     `arg:"list" json:"list,omitempty"`
	Timestamps bool     `arg:"timestamps" json:"timestamps,omitempty"`
	Count      bool     `arg:"count" json:"count,omitempty"`
	Repair     bool     `arg:"repair" json:"repair,omitempty"`
	Check      bool     `arg:"check" json:"check,omitempty"`
	Update     bool     `arg:"update" json:"update,omitempty"`
	Deep       bool     `arg:"deep" json:"deep,omitempty"`
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// String implements the stringer interface
func (opts *WhenOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// When implements the chifra when command for the SDK.
func (opts *WhenOptions) When(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	for _, blockId := range opts.BlockIds {
		values.Add("blocks", blockId)
	}
	if opts.List {
		values.Set("list", "true")
	}
	if opts.Timestamps {
		values.Set("timestamps", "true")
	}
	if opts.Count {
		values.Set("count", "true")
	}
	if opts.Repair {
		values.Set("repair", "true")
	}
	if opts.Check {
		values.Set("check", "true")
	}
	if opts.Update {
		values.Set("update", "true")
	}
	if opts.Deep {
		values.Set("deep", "true")
	}
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return when.When(w, values)
}

// GetWhenOptions returns a filled-in options instance given a string array of arguments.
func GetWhenOptions(args []string) (*WhenOptions, error) {
	var opts WhenOptions
	if err := assignValuesFromArgs(&opts, &opts.Globals, args); err != nil {
		return nil, err
	}
	return &opts, nil
}

// No enums

// EXISTING_CODE
// EXISTING_CODE

