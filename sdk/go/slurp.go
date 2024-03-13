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

	slurp "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type SlurpOptions struct {
	Addrs       []string // allow for ENS names and addresses
	BlockIds    []string // allow for block ranges and steps
	Types       SlurpTypes
	Appearances bool
	Articulate  bool
	Source      SlurpSource
	Count       bool
	Sleep       float64
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
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
	STSome
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
		"some",
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

