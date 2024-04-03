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
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"log"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	tokens "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
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

// Tokens implements the chifra tokens command for the SDK.
func (opts *TokensOptions) Tokens(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		log.Fatalf("Error converting tokens struct to URL values: %v", err)
	}

	return tokens.Tokens(w, values)
}

// tokensParseFunc handles specail cases such as structs and enums (if any).
func tokensParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	opts, ok := target.(*TokensOptions)
	if !ok {
		return false, fmt.Errorf("parseFunc(tokens): target is not of correct type")
	}

	if key == "parts" {
		var err error
		values := strings.Split(value, ",")
		if opts.Parts, err = enumFromTokensParts(values); err != nil {
			return false, err
		} else {
			found = true
		}
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// GetTokensOptions returns a filled-in options instance given a string array of arguments.
func GetTokensOptions(args []string) (*TokensOptions, error) {
	var opts TokensOptions
	if err := assignValuesFromArgs(args, tokensParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

func (opts *TokensOptions) Query() ([]bool, *rpc.MetaData, error) {
	buffer := bytes.Buffer{}
	if err := opts.Tokens(&buffer); err != nil {
		logger.Fatal(err)
	}

	var result Result[bool]
	if err := json.Unmarshal(buffer.Bytes(), &result); err != nil {
		return nil, nil, err
	} else {
		return result.Data, &result.Meta, nil
	}
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
