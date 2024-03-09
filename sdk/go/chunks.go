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
	"encoding/json"
	"fmt"
	"io"
	"net/url"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	chunks "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type ChunksOptions struct {
	Mode       ChunksMode   `json:"mode,omitempty"`
	BlockIds   []string     `json:"blocks,omitempty"`
	Check      bool         `json:"check,omitempty"`
	Pin        bool         `json:"pin,omitempty"`
	Publish    bool         `json:"publish,omitempty"`
	Remote     bool         `json:"remote,omitempty"`
	Belongs    []string     `json:"belongs,omitempty"`
	FirstBlock base.Blknum  `json:"firstBlock,omitempty"`
	LastBlock  base.Blknum  `json:"lastBlock,omitempty"`
	MaxAddrs   base.Blknum  `json:"maxAddrs,omitempty"`
	Deep       bool         `json:"deep,omitempty"`
	Rewrite    bool         `json:"rewrite,omitempty"`
	Count      bool         `json:"count,omitempty"`
	Sleep      float64      `json:"sleep,omitempty"`
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// String implements the stringer interface
func (opts *ChunksOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Chunks implements the chifra chunks command for the SDK.
func (opts *ChunksOptions) Chunks(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	if opts.Mode != NoCM2 {
		values.Set("mode", opts.Mode.String())
	}
	for _, v := range opts.BlockIds {
		values.Add("blocks", v)
	}
	if opts.Check {
		values.Set("check", "true")
	}
	if opts.Pin {
		values.Set("pin", "true")
	}
	if opts.Publish {
		values.Set("publish", "true")
	}
	if opts.Remote {
		values.Set("remote", "true")
	}
	for _, v := range opts.Belongs {
		values.Add("belongs", v)
	}
	if opts.FirstBlock > 0 {
		values.Set("first_block", fmt.Sprint(opts.FirstBlock))
	}
	if opts.LastBlock > 0 {
		values.Set("last_block", fmt.Sprint(opts.LastBlock))
	}
	if opts.MaxAddrs > 0 {
		values.Set("max_addrs", fmt.Sprint(opts.MaxAddrs))
	}
	if opts.Deep {
		values.Set("deep", "true")
	}
	if opts.Rewrite {
		values.Set("rewrite", "true")
	}
	if opts.Count {
		values.Set("count", "true")
	}
	if opts.Sleep > 0 {
		values.Set("sleep", fmt.Sprint(opts.Sleep))
	}
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return chunks.Chunks(w, values)
}

// GetChunksOptions returns a filled-in options instance given a string array of arguments.
func GetChunksOptions(args []string) (*ChunksOptions, error) {
	var opts ChunksOptions
	if err := assignValuesFromArgs(&opts, &opts.Globals, args); err != nil {
		return nil, err
	}
	return &opts, nil
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

