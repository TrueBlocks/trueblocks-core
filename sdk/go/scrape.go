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
	"log"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	scrape "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type ScrapeOptions struct {
	BlockCnt  uint64       `json:"blockCnt,omitempty"`
	Sleep     float64      `json:"sleep,omitempty"`
	Touch     uint64       `json:"touch,omitempty"`
	RunCount  uint64       `json:"runCount,omitempty"`
	Publisher base.Address `json:"publisher,omitempty"`
	DryRun    bool         `json:"dryRun,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *ScrapeOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Scrape implements the chifra scrape command for the SDK.
func (opts *ScrapeOptions) Scrape(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		log.Fatalf("Error converting scrape struct to URL values: %v", err)
	}

	return scrape.Scrape(w, values)
}

// scrapeParseFunc handles specail cases such as structs and enums (if any).
func scrapeParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	_, ok := target.(*ScrapeOptions)
	if !ok {
		return false, fmt.Errorf("parseFunc(scrape): target is not of correct type")
	}

	// No enums

	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// GetScrapeOptions returns a filled-in options instance given a string array of arguments.
func GetScrapeOptions(args []string) (*ScrapeOptions, error) {
	var opts ScrapeOptions
	if err := assignValuesFromArgs(args, scrapeParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

// No enums
