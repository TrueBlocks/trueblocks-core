// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package sdk

import (
	// EXISTING_CODE
	"encoding/json"
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	// EXISTING_CODE
)

type TokensOptions struct {
	Addrs    []string    `json:"addrs,omitempty"`
	BlockIds []string    `json:"blocks,omitempty"`
	Parts    TokensParts `json:"parts,omitempty"`
	ByAcct   bool        `json:"byAcct,omitempty"`
	Changes  bool        `json:"changes,omitempty"`
	NoZero   bool        `json:"noZero,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *TokensOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Tokens implements the chifra tokens command.
func (opts *TokensOptions) Tokens() ([]types.Token, *types.MetaData, error) {
	in := opts.toInternal()
	return queryTokens[types.Token](in)
}

type TokensParts int

const (
	NoTP   TokensParts = 0
	TPName             = 1 << iota
	TPSymbol
	TPDecimals
	TPTotalSupply
	TPVersion
	TPSome = TPName | TPSymbol | TPDecimals | TPTotalSupply
	TPAll  = TPName | TPSymbol | TPDecimals | TPTotalSupply | TPVersion
)

func (v TokensParts) String() string {
	switch v {
	case NoTP:
		return "none"
	case TPSome:
		return "some"
	case TPAll:
		return "all"
	}

	var m = map[TokensParts]string{
		TPName:        "name",
		TPSymbol:      "symbol",
		TPDecimals:    "decimals",
		TPTotalSupply: "totalSupply",
		TPVersion:     "version",
	}

	var ret []string
	for _, val := range []TokensParts{TPName, TPSymbol, TPDecimals, TPTotalSupply, TPVersion} {
		if v&val != 0 {
			ret = append(ret, m[val])
		}
	}

	return strings.Join(ret, ",")
}

func enumFromTokensParts(values []string) (TokensParts, error) {
	if len(values) == 0 {
		return NoTP, fmt.Errorf("no value provided for parts option")
	}

	if len(values) == 1 && values[0] == "all" {
		return TPAll, nil
	} else if len(values) == 1 && values[0] == "some" {
		return TPSome, nil
	}

	var result TokensParts
	for _, val := range values {
		switch val {
		case "name":
			result |= TPName
		case "symbol":
			result |= TPSymbol
		case "decimals":
			result |= TPDecimals
		case "totalSupply":
			result |= TPTotalSupply
		case "version":
			result |= TPVersion
		default:
			return NoTP, fmt.Errorf("unknown parts: %s", val)
		}
	}

	return result, nil
}

// EXISTING_CODE
// EXISTING_CODE
