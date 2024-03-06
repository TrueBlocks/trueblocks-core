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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
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

// GetListOptions returns an options instance given a string array of arguments.
func GetListOptions(args []string) (*ListOptions, error) {
	var opts ListOptions

	for i, arg := range args {
		// EXISTING_CODE
		logger.Info(fmt.Sprintf("\t%d: %s\n", i, arg))
		// opt := strings.Split(arg, "=")
		// switch opt[0] {
		// case "@b", "bounds":
		// 	opts.Bounds = true
		// case "@c", "first_record":
		// 	opts.FirstRecord = utils.MustParseUint(opt[1])
		// case "@e", "max_records":
		// 	opts.MaxRecords = utils.MustParseUint(opt[1])
		// case "@E", "reversed":
		// 	opts.Reversed = true
		// case "@F", "first_block":
		// 	opts.FirstBlock = utils.MustParseUint(opt[1])
		// case "@h", "help":
		// case "@L", "last_block":
		// 	opts.LastBlock = utils.MustParseUint(opt[1])
		// case "@P", "publisher":
		// 	// opts.Publisher = opt[1]
		// case "@s", "silent":
		// 	opts.Silent = true
		// case "@U", "count":
		// 	opts.Count = true
		// case "@u", "unripe":
		// 	opts.Unripe = true
		// case "@v", "verbose":
		// 	opts.Verbose = true
		// // case "@x", "fmt string":
		// // 	opts.Fmt = opt[1]
		// case "@z", "no_zero":
		// 	opts.NoZero = true
		// default:
		// 	return &opts, fmt.Errorf("unknown option: %s", opt[0])
		// }
		// EXISTING_CODE
	}

	return &opts, nil
}

// No enums

// EXISTING_CODE
// EXISTING_CODE

