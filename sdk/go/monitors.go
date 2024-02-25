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
	//   addrs - one or more addresses (0x...) to process
	//       --delete             delete a monitor, but do not remove it
	//       --undelete           undelete a previously deleted monitor
	//       --remove             remove a previously deleted monitor
	//   -C, --clean              clean (i.e. remove duplicate appearances) from monitors
	//   -l, --list               list monitors in the cache (--verbose for more detail)
	//   -w, --watch              continually scan for new blocks and extract data as per the command file
	//   -a, --watchlist string   available with --watch option only, a file containing the addresses to watch
	//   -c, --commands string    available with --watch option only, the file containing the list of commands to apply to each watched address
	//   -b, --batch_size uint    available with --watch option only, the number of monitors to process in each batch (default 8)
	//   -s, --sleep float        available with --watch option only, the number of seconds to sleep between runs (default 14)
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return monitors.Monitors(w, values)
}

// no enums

// EXISTING_CODE
// EXISTING_CODE

