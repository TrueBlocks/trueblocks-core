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
	list "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type ListOptions struct {
	Addrs       []string // allow for ENS names and addresses
	Count       bool
	NoZero      bool
	Bounds      bool
	Unripe      bool
	Silent      bool
	FirstRecord uint64
	MaxRecords  uint64
	Reversed    bool
	Publisher   base.Address
	FirstBlock  base.Blknum
	LastBlock   base.Blknum
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// List implements the chifra list command for the SDK.
func (opts *ListOptions) List(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	//   addrs - one or more addresses (0x...) to list (required)
	//   -U, --count               display only the count of records for each monitor
	//   -z, --no_zero             for the --count option only, suppress the display of zero appearance accounts
	//   -b, --bounds              report first and last block this address appears
	//   -u, --unripe              list transactions labeled upripe (i.e. less than 28 blocks old)
	//   -s, --silent              freshen the monitor only (no reporting)
	//   -c, --first_record uint   the first record to process
	//   -e, --max_records uint    the maximum number of records to process (default 250)
	//   -E, --reversed            produce results in reverse chronological order
	//   -F, --first_block uint    first block to export (inclusive, ignored when freshening)
	//   -L, --last_block uint     last block to export (inclusive, ignored when freshening)
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return list.List(w, values)
}

// no enums

// EXISTING_CODE
// EXISTING_CODE

