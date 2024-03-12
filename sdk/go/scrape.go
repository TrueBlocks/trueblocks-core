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

	scrape "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type ScrapeOptions struct {
	BlockCnt uint64  `json:"blockCnt,omitempty"`
	Sleep    float64 `json:"sleep,omitempty"`
	Touch    uint64  `json:"touch,omitempty"`
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// String implements the stringer interface
func (opts *ScrapeOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Scrape implements the chifra scrape command for the SDK.
func (opts *ScrapeOptions) Scrape(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	if opts.BlockCnt != 0 {
		values.Set("block_cnt", fmt.Sprint(opts.BlockCnt))
	}
	if opts.Sleep != 0 {
		values.Set("sleep", fmt.Sprint(opts.Sleep))
	}
	if opts.Touch != 0 {
		values.Set("touch", fmt.Sprint(opts.Touch))
	}
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return scrape.Scrape(w, values)
}

// GetScrapeOptions returns a filled-in options instance given a string array of arguments.
func GetScrapeOptions(args []string) (*ScrapeOptions, error) {
	var opts ScrapeOptions
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
