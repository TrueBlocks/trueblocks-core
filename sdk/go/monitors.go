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

	monitors "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type MonitorsOptions struct {
	Addrs     []string // allow for ENS names and addresses
	Delete    bool
	Undelete  bool
	Remove    bool
	Clean     bool
	List      bool
	Watch     bool
	Watchlist string
	Commands  string
	BatchSize uint64
	RunCount  uint64
	Sleep     float64
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// Monitors implements the chifra monitors command for the SDK.
func (opts *MonitorsOptions) Monitors(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	// EXISTING_CODE

	return monitors.Monitors(w, values)
}

// no enums

// EXISTING_CODE
// EXISTING_CODE

