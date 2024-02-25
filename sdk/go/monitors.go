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
	for _, v := range opts.Addrs {
		values.Add("addrs", v)
	}
	if opts.Delete {
		values.Set("delete", "true")
	}
	if opts.Undelete {
		values.Set("undelete", "true")
	}
	if opts.Remove {
		values.Set("remove", "true")
	}
	if opts.Clean {
		values.Set("clean", "true")
	}
	if opts.List {
		values.Set("list", "true")
	}
	if opts.Watch {
		values.Set("watch", "true")
	}
	if opts.Watchlist != "" {
		values.Set("watchlist", opts.Watchlist)
	}
	if opts.Commands != "" {
		values.Set("commands", opts.Commands)
	}
	if opts.BatchSize > 0 {
		values.Set("batch_size", fmt.Sprint(opts.BatchSize))
	}
	if opts.RunCount > 0 {
		values.Set("run_count", fmt.Sprint(opts.RunCount))
	}
	if opts.Sleep > 0 {
		values.Set("sleep", fmt.Sprint(opts.Sleep))
	}
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return monitors.Monitors(w, values)
}

// no enums

// EXISTING_CODE
// EXISTING_CODE
