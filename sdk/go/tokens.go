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
	//   addrs - two or more addresses (0x...), the first is an ERC20 token, balances for the rest are reported (required)
	//   blocks - an optional list of one or more blocks at which to report balances, defaults to 'latest'
	//   -p, --parts strings   which parts of the token information to retrieve
	//                         One or more of [ name | symbol | decimals | totalSupply | version | all ]
	//   -b, --by_acct         consider each address an ERC20 token except the last, whose balance is reported for each token
	//   -c, --changes         only report a balance when it changes from one block to the next
	//   -z, --no_zero         suppress the display of zero balance accounts
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

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

