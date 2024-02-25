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
	// EXISTING_CODE

	return scrape.Scrape(w, values)
}

// no enums

// EXISTING_CODE
// EXISTING_CODE

