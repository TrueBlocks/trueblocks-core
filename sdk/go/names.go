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
	names "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type NamesOptions struct {
	Terms     []string
	Expand    bool
	MatchCase bool
	All       bool
	Custom    bool
	Prefund   bool
	Addr      bool
	Tags      bool
	Clean     bool
	Regular   bool
	DryRun    bool
	Autoname  base.Address
	Create    bool
	Update    bool
	Delete    bool
	Undelete  bool
	Remove    bool
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// Names implements the chifra names command for the SDK.
func (opts *NamesOptions) Names(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	//   terms - a space separated list of one or more search terms (required)
	//   -e, --expand            expand search to include all fields (search name, address, and symbol otherwise)
	//   -m, --match_case        do case-sensitive search
	//   -a, --all               include all (including custom) names in the search
	//   -c, --custom            include only custom named accounts in the search
	//   -p, --prefund           include prefund accounts in the search
	//   -s, --addr              display only addresses in the results (useful for scripting, assumes --no_header)
	//   -g, --tags              export the list of tags and subtags only
	//   -C, --clean             clean the data (addrs to lower case, sort by addr)
	//   -r, --regular           only available with --clean, cleans regular names database
	//   -d, --dry_run           only available with --clean or --autoname, outputs changes to stdout instead of updating databases
	//   -A, --autoname string   an address assumed to be a token, added automatically to names database if true
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return names.Names(w, values)
}

// no enums

// EXISTING_CODE
// EXISTING_CODE

