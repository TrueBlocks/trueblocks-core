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
	"io"
	"net/url"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	scrape "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type ScrapeOptions struct {
	BlockCnt  uint64
	Sleep     float64
	Touch     uint64
	RunCount  uint64
	Publisher base.Address
	DryRun    bool
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// Scrape implements the chifra scrape command for the SDK.
func (opts *ScrapeOptions) Scrape(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	//   -n, --block_cnt uint   maximum number of blocks to process per pass (default 2000)
	//   -s, --sleep float      seconds to sleep between scraper passes (default 14)
	//   -l, --touch uint       first block to visit when scraping (snapped back to most recent snap_to_grid mark)
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return scrape.Scrape(w, values)
}

// no enums

// EXISTING_CODE
// EXISTING_CODE

