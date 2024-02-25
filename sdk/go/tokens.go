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

	tokens "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type TokensOptions struct {
	Addrs    []string // allow for ENS names and addresses
	BlockIds []string // allow for block ranges and steps
	Parts    TokensParts
	ByAcct   bool
	Changes  bool
	NoZero   bool
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// Tokens implements the chifra tokens command for the SDK.
func (opts *TokensOptions) Tokens(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	// EXISTING_CODE

	return tokens.Tokens(w, values)
}

type TokensParts int

const (
	NoTP TokensParts = iota
	TPName
	TPSymbol
	TPDecimals
	TPTotalSupply
	TPVersion
	TPAll
)

func (v TokensParts) String() string {
	return []string{
		"notp",
		"name",
		"symbol",
		"decimals",
		"totalsupply",
		"version",
		"all",
	}[v]
}

// EXISTING_CODE
// EXISTING_CODE

