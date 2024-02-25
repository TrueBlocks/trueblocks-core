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
	initPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type InitOptions struct {
	All        bool
	DryRun     bool
	Publisher  base.Address
	FirstBlock base.Blknum
	Sleep      float64
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// Init implements the chifra init command for the SDK.
func (opts *InitOptions) Init(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	//   chifra init [flags]
	//   -a, --all                in addition to Bloom filters, download full index chunks (recommended)
	//   -d, --dry_run            display the results of the download without actually downloading
	//   -F, --first_block uint   do not download any chunks earlier than this block
	//   -s, --sleep float        seconds to sleep between downloads
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return initPkg.Init(w, values)
}

// no enums

// EXISTING_CODE
// EXISTING_CODE

