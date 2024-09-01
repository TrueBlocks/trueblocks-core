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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	// EXISTING_CODE
)

type ScrapeOptions struct {
	BlockCnt  uint64            `json:"blockCnt,omitempty"`
	Sleep     float64           `json:"sleep,omitempty"`
	Publisher base.Address      `json:"publisher,omitempty"`
	Notify    bool              `json:"notify,omitempty"`
	RenderCtx *output.RenderCtx `json:"-"`
	Globals
}

// String implements the stringer interface
func (opts ScrapeOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// ScrapeTouch implements the chifra scrape --touch command.
func (opts *ScrapeOptions) ScrapeTouch(val base.Blknum) ([]types.Message, *types.MetaData, error) {
	in := opts.toInternal()
	in.Touch = val
	return queryScrape[types.Message](in)
}

// ScrapeRunCount implements the chifra scrape --runcount command.
func (opts *ScrapeOptions) ScrapeRunCount(val uint64) ([]types.Message, *types.MetaData, error) {
	in := opts.toInternal()
	in.RunCount = val
	return queryScrape[types.Message](in)
}

// ScrapeDryRun implements the chifra scrape --dryrun command.
func (opts *ScrapeOptions) ScrapeDryRun() ([]types.Message, *types.MetaData, error) {
	in := opts.toInternal()
	in.DryRun = true
	return queryScrape[types.Message](in)
}

// No enums
// EXISTING_CODE
func (opts *ScrapeOptions) ScrapeRunOnce() ([]types.Message, *types.MetaData, error) {
	return opts.ScrapeRunCount(1)
}

// EXISTING_CODE
