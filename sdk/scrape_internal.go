// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package sdk

import (
	// EXISTING_CODE
	"encoding/json"
	"fmt"
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	scrape "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type scrapeOptionsInternal struct {
	BlockCnt  uint64            `json:"blockCnt,omitempty"`
	Sleep     float64           `json:"sleep,omitempty"`
	Touch     base.Blknum       `json:"touch,omitempty"`
	RunCount  uint64            `json:"runCount,omitempty"`
	Publisher base.Address      `json:"publisher,omitempty"`
	DryRun    bool              `json:"dryRun,omitempty"`
	Notify    bool              `json:"notify,omitempty"`
	RenderCtx *output.RenderCtx `json:"-"`
	Globals
}

// String implements the stringer interface
func (opts *scrapeOptionsInternal) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// ScrapeBytes implements the chifra scrape command for the SDK.
func (opts *scrapeOptionsInternal) ScrapeBytes(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		return fmt.Errorf("error converting scrape struct to URL values: %v", err)
	}

	if opts.RenderCtx == nil {
		opts.RenderCtx = output.NewRenderContext()
	}
	return scrape.Scrape(opts.RenderCtx, w, values)
}

// scrapeParseFunc handles special cases such as structs and enums (if any).
func scrapeParseFunc(target any, key, value string) (bool, error) {
	var found bool
	_, ok := target.(*scrapeOptionsInternal)
	if !ok {
		return false, fmt.Errorf("parseFunc(scrape): target is not of correct type")
	}

	// No enums
	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// GetScrapeOptions returns a filled-in options instance given a string array of arguments.
func GetScrapeOptions(args []string) (*scrapeOptionsInternal, error) {
	var opts scrapeOptionsInternal
	if err := assignValuesFromArgs(args, scrapeParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

// EXISTING_CODE
// EXISTING_CODE
