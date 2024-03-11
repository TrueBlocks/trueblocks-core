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

	slurp "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type SlurpOptions struct {
	Addrs       []string    `json:"addrs,omitempty"`
	BlockIds    []string    `json:"blocks,omitempty"`
	Types       SlurpTypes  `json:"types,omitempty"`
	Appearances bool        `json:"appearances,omitempty"`
	Articulate  bool        `json:"articulate,omitempty"`
	Source      SlurpSource `json:"source,omitempty"`
	Count       bool        `json:"count,omitempty"`
	Sleep       float64     `json:"sleep,omitempty"`
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// String implements the stringer interface
func (opts *SlurpOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Slurp implements the chifra slurp command for the SDK.
func (opts *SlurpOptions) Slurp(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	for _, v := range opts.Addrs {
		values.Add("addrs", v)
	}
	for _, v := range opts.BlockIds {
		values.Add("blocks", v)
	}
	if opts.Types != NoST {
		values.Set("types", opts.Types.String())
	}
	if opts.Appearances {
		values.Set("appearances", "true")
	}
	if opts.Articulate {
		values.Set("articulate", "true")
	}
	if opts.Source != NoSS {
		values.Set("source", opts.Source.String())
	}
	if opts.Count {
		values.Set("count", "true")
	}
	if opts.Sleep > 0 {
		values.Set("sleep", fmt.Sprint(opts.Sleep))
	}
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return slurp.Slurp(w, values)
}

// GetSlurpOptions returns a filled-in options instance given a string array of arguments.
func GetSlurpOptions(args []string) (*SlurpOptions, error) {
	var opts SlurpOptions
	if err := assignValuesFromArgs(&opts, &opts.Globals, args); err != nil {
		return nil, err
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return &opts, nil
}

type SlurpTypes int

const (
	NoST SlurpTypes = iota
	STExt
	STInt
	STToken
	STNfts
	ST1155
	STMiner
	STUncles
	STWithdrawals
	STAll
)

func (v SlurpTypes) String() string {
	return []string{
		"nost",
		"ext",
		"int",
		"token",
		"nfts",
		"1155",
		"miner",
		"uncles",
		"withdrawals",
		"all",
	}[v]
}

type SlurpSource int

const (
	NoSS SlurpSource = iota
	SSEtherscan
	SSKey
)

func (v SlurpSource) String() string {
	return []string{
		"noss",
		"etherscan",
		"key",
	}[v]
}

// EXISTING_CODE
// EXISTING_CODE

