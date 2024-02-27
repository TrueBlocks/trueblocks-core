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
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
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
	for _, v := range opts.Addrs {
		values.Add("addrs", v)
	}
	if opts.Count {
		values.Set("count", "true")
	}
	if opts.NoZero {
		values.Set("no_zero", "true")
	}
	if opts.Bounds {
		values.Set("bounds", "true")
	}
	if opts.Unripe {
		values.Set("unripe", "true")
	}
	if opts.Silent {
		values.Set("silent", "true")
	}
	if opts.FirstRecord != 0 {
		values.Set("first_record", fmt.Sprint(opts.FirstRecord))
	}
	if opts.MaxRecords != 0 {
		values.Set("max_records", fmt.Sprint(opts.MaxRecords))
	}
	if opts.Reversed {
		values.Set("reversed", "true")
	}
	if opts.FirstBlock > 0 {
		values.Set("first_block", fmt.Sprint(opts.FirstBlock))
	}
	if opts.LastBlock > 0 {
		values.Set("last_block", fmt.Sprint(opts.LastBlock))
	}
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return list.List(w, values)
}

// EXISTING_CODE
func GetListOpts(args []string) (lo ListOptions, err error) {
	for _, arg := range args {
		opt := strings.Split(arg, "=")
		switch opt[0] {
		case "@b", "bounds":
			lo.Bounds = true
		case "@c", "first_record":
			lo.FirstRecord = utils.MustParseUint(opt[1])
		case "@e", "max_records":
			lo.MaxRecords = utils.MustParseUint(opt[1])
		case "@E", "reversed":
			lo.Reversed = true
		case "@F", "first_block":
			lo.FirstBlock = utils.MustParseUint(opt[1])
		case "@h", "help":
		case "@L", "last_block":
			lo.LastBlock = utils.MustParseUint(opt[1])
		case "@P", "publisher":
			// lo.Publisher = opt[1]
		case "@s", "silent":
			lo.Silent = true
		case "@U", "count":
			lo.Count = true
		case "@u", "unripe":
			lo.Unripe = true
		case "@v", "verbose":
			lo.Verbose = true
		// case "@x", "fmt string":
		// 	lo.Fmt = opt[1]
		case "@z", "no_zero":
			lo.NoZero = true
		default:
			return lo, fmt.Errorf("unknown option: %s", opt[0])
		}
	}
	return lo, nil
}

// EXISTING_CODE
