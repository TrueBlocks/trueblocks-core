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

	list "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

// ListOptions represents the command line options for the chifra list command
type ListOptions struct {
	Addrs       []string    `arg:"addrs,omitempty" json:"addrs,omitempty"`
	Count       bool        `arg:"count,omitempty" json:"count,omitempty"`
	NoZero      bool        `arg:"noZero,omitempty" json:"noZero,omitempty"`
	Bounds      bool        `arg:"bounds,omitempty" json:"bounds,omitempty"`
	Unripe      bool        `arg:"unripe,omitempty" json:"unripe,omitempty"`
	Silent      bool        `arg:"silent,omitempty" json:"silent,omitempty"`
	FirstRecord uint64      `arg:"firstRecord,omitempty" json:"firstRecord,omitempty"`
	MaxRecords  uint64      `arg:"maxRecords,omitempty" json:"maxRecords,omitempty"`
	Reversed    bool        `arg:"reversed,omitempty" json:"reversed,omitempty"`
	Publisher   string      `arg:"publisher,omitempty" json:"publisher,omitempty"`
	FirstBlock  base.Blknum `arg:"firstBlock,omitempty" json:"firstBlock,omitempty"`
	LastBlock   base.Blknum `arg:"lastBlock,omitempty" json:"lastBlock,omitempty"`
	Globals
}

func (opts *ListOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
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
	if opts.Publisher != "" {
		values.Set("publisher", opts.Publisher)
	}
	if opts.FirstBlock > 0 {
		values.Set("firstBlock", fmt.Sprint(opts.FirstBlock))
	}
	if opts.LastBlock > 0 {
		values.Set("lastBlock", fmt.Sprint(opts.LastBlock))
	}
	// EXISTING_CODE
	// opts.Globals.mapGlobals(values)

	return list.List(w, values)
}

// GetListOptions returns an options instance given a string array of arguments.
func GetListOptions(args []string) (*ListOptions, error) {
	var err error
	var opts ListOptions
	err := assignValuesFromArgs(&opts, &opts.Globals, args)
	logger.Info("Args:", args)
	logger.Info("Opts:", opts.String())
	if err != nil {
		return nil, err
	}

	return &opts, nil
}

// No enums

// EXISTING_CODE
// EXISTING_CODE
