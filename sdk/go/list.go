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

	list "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

// ListOptions represents the command line options for the chifra list command
type ListOptions struct {
	Count       bool     `flag:"count,U"`
	NoZero      bool     `flag:"noZero,z"`
	Bounds      bool     `flag:"bounds,b"`
	Unripe      bool     `flag:"unripe,u"`
	Silent      bool     `flag:"silent,s"`
	FirstRecord uint     `flag:"firstRecord,c"`
	MaxRecords  uint     `flag:"maxRecords,e"`
	Reversed    bool     `flag:"reversed,E"`
	Publisher   string   `flag:"publisher,P"`
	FirstBlock  uint     `flag:"firstBlock,F"`
	LastBlock   uint     `flag:"lastBlock,L"`
	Format      string   `flag:"fmt,x"`
	Verbose     bool     `flag:"verbose,v"`
	Help        bool     `flag:"help,h"`
	Positionals []string // Stores positional arguments (addresses)
	Globals
}

// List implements the chifra list command for the SDK.
func (opts *ListOptions) List(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	for _, v := range opts.Positionals {
		values.Add("addrs", v)
	}
	if opts.Count {
		values.Set("count", "true")
	}
	if opts.NoZero {
		values.Set("no_zero", "true")
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
		values.Set("first_record", fmt.Sprint(opts.FirstRecord))
	}
	if opts.MaxRecords != 0 {
		values.Set("max_records", fmt.Sprint(opts.MaxRecords))
	}
	if opts.Reversed {
		values.Set("reversed", "true")
	}
	if opts.Publisher != "" {
		values.Set("publisher", opts.Publisher)
	}
	if opts.FirstBlock > 0 {
		values.Set("first_block", fmt.Sprint(opts.FirstBlock))
	}
	if opts.LastBlock > 0 {
		values.Set("last_block", fmt.Sprint(opts.LastBlock))
	}
	// EXISTING_CODE
	// opts.Globals.mapGlobals(values)

	return list.List(w, values)
}

// GetListOptions returns an options instance given a string array of arguments.
func GetListOptions(args []string) (*ListOptions, error) {
	var err error
	var opts ListOptions
	opts.Positionals, err = ParseOptions[ListOptions](args, &opts)
	return &opts, err
}

// EXISTING_CODE
// EXISTING_CODE
