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
	"bytes"
	"encoding/json"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	// EXISTING_CODE
)

type ScrapeOptions struct {
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
func (opts ScrapeOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// No enums
// EXISTING_CODE
func (opts *ScrapeOptions) Scrape() error {
	in := opts.toInternal()
	buffer := bytes.Buffer{}
	return in.ScrapeBytes(&buffer)
}

func (opts *ScrapeOptions) toInternal() *scrapeOptionsInternal {
	return &scrapeOptionsInternal{
		BlockCnt:  opts.BlockCnt,
		Sleep:     opts.Sleep,
		Touch:     opts.Touch,
		RunCount:  opts.RunCount,
		Publisher: opts.Publisher,
		DryRun:    opts.DryRun,
		Notify:    opts.Notify,
	}
}

// // ScrapeBytes implements the chifra slurp command for the SDK.
// func (opts *scrapeOptionsInternal) ScrapeBytes(w io.Writer) error {
// 	values, err := structToValues(*opts)
// 	if err != nil {
// 		return fmt.Errorf("error converting scrape struct to URL values: %v", err)
// 	}
// 	if opts.RenderCtx == nil {
// 		opts.RenderCtx = output.NewRenderContext()
// 	}
// 	return scrape.Scrape(opts.RenderCtx, w, values)
// }

// EXISTING_CODE
