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
	chunks "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type ChunksOptions struct {
	Mode       ChunksMode
	BlockIds   []string // allow for block ranges and steps
	Check      bool
	Pin        bool
	Publish    bool
	Publisher  base.Address
	Truncate   base.Blknum
	Remote     bool
	Belongs    []string // allow for ENS names and addresses
	Diff       bool
	FirstBlock base.Blknum
	LastBlock  base.Blknum
	MaxAddrs   base.Blknum
	Deep       bool
	Rewrite    bool
	List       bool
	Unpin      bool
	Count      bool
	Tag        string
	Sleep      float64
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// Chunks implements the chifra chunks command for the SDK.
func (opts *ChunksOptions) Chunks(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	//   mode - the type of data to process (required)
	// 	One of [ manifest | index | blooms | pins | addresses | appearances | stats ]
	//   blocks - an optional list of blocks to intersect with chunk ranges
	//   -c, --check              check the manifest, index, or blooms for internal consistency
	//   -i, --pin                pin the manifest or each index chunk and bloom
	//   -p, --publish            publish the manifest to the Unchained Index smart contract
	//   -r, --remote             prior to processing, retrieve the manifest from the Unchained Index smart contract
	//   -b, --belongs strings    in index mode only, checks the address(es) for inclusion in the given index chunk
	//   -F, --first_block uint   first block to process (inclusive)
	//   -L, --last_block uint    last block to process (inclusive)
	//   -m, --max_addrs uint     the max number of addresses to process in a given chunk
	//   -d, --deep               if true, dig more deeply during checking (manifest only)
	//   -e, --rewrite            for the --pin --deep mode only, writes the manifest back to the index folder (see notes)
	//   -U, --count              for the pins mode only, display only the count of records
	//   -s, --sleep float        for --remote pinning only, seconds to sleep between API calls
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return chunks.Chunks(w, values)
}

type ChunksMode int

const (
	NoCM2 ChunksMode = iota
	CMManifest
	CMIndex
	CMBlooms
	CMPins
	CMAddresses
	CMAppearances
	CMStats
)

func (v ChunksMode) String() string {
	return []string{
		"nocm2",
		"manifest",
		"index",
		"blooms",
		"pins",
		"addresses",
		"appearances",
		"stats",
	}[v]
}

// EXISTING_CODE
// EXISTING_CODE

