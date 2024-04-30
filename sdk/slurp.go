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

type SlurpOptions struct {
	Addrs      []string    `json:"addrs,omitempty"`
	BlockIds   []string    `json:"blocks,omitempty"`
	Types      SlurpTypes  `json:"types,omitempty"`
	Articulate bool        `json:"articulate,omitempty"`
	Source     SlurpSource `json:"source,omitempty"`
	Page       uint64      `json:"page,omitempty"`
	PerPage    uint64      `json:"perPage,omitempty"`
	Sleep      float64     `json:"sleep,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts SlurpOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Slurp implements the chifra slurp command.
func (opts *SlurpOptions) Slurp() ([]types.Slurp, *types.MetaData, error) {
	in := opts.toInternal()
	return querySlurp[types.Slurp](in)
}

// SlurpAppearances implements the chifra slurp --appearances command.
func (opts *SlurpOptions) SlurpAppearances() ([]types.Appearance, *types.MetaData, error) {
	in := opts.toInternal()
	in.Appearances = true
	return querySlurp[types.Appearance](in)
}

// SlurpCount implements the chifra slurp --count command.
func (opts *SlurpOptions) SlurpCount() ([]types.SlurpCount, *types.MetaData, error) {
	in := opts.toInternal()
	in.Count = true
	return querySlurp[types.SlurpCount](in)
}

type SlurpTypes int

const (
	NoST  SlurpTypes = 0
	STExt            = 1 << iota
	STInt
	STToken
	STNfts
	ST1155
	STMiner
	STUncles
	STWithdrawals
	STSome = STExt | STInt | STToken | STNfts
	STAll  = STExt | STInt | STToken | STNfts | ST1155 | STMiner | STUncles | STWithdrawals
)

func (v SlurpTypes) String() string {
	switch v {
	case NoST:
		return "none"
	case STSome:
		return "some"
	case STAll:
		return "all"
	}

	var m = map[SlurpTypes]string{
		STExt:         "ext",
		STInt:         "int",
		STToken:       "token",
		STNfts:        "nfts",
		ST1155:        "1155",
		STMiner:       "miner",
		STUncles:      "uncles",
		STWithdrawals: "withdrawals",
	}

	var ret []string
	for _, val := range []SlurpTypes{STExt, STInt, STToken, STNfts, ST1155, STMiner, STUncles, STWithdrawals} {
		if v&val != 0 {
			ret = append(ret, m[val])
		}
	}

	return strings.Join(ret, ",")
}

func enumFromSlurpTypes(values []string) (SlurpTypes, error) {
	if len(values) == 0 {
		return NoST, fmt.Errorf("no value provided for types option")
	}

	if len(values) == 1 && values[0] == "all" {
		return STAll, nil
	} else if len(values) == 1 && values[0] == "some" {
		return STSome, nil
	}

	var result SlurpTypes
	for _, val := range values {
		switch val {
		case "ext":
			result |= STExt
		case "int":
			result |= STInt
		case "token":
			result |= STToken
		case "nfts":
			result |= STNfts
		case "1155":
			result |= ST1155
		case "miner":
			result |= STMiner
		case "uncles":
			result |= STUncles
		case "withdrawals":
			result |= STWithdrawals
		default:
			return NoST, fmt.Errorf("unknown types: %s", val)
		}
	}

	return result, nil
}

type SlurpSource int

const (
	NoSS        SlurpSource = 0
	SSEtherscan             = 1 << iota
	SSKey
)

func (v SlurpSource) String() string {
	switch v {
	case NoSS:
		return "none"
	}

	var m = map[SlurpSource]string{
		SSEtherscan: "etherscan",
		SSKey:       "key",
	}

	var ret []string
	for _, val := range []SlurpSource{SSEtherscan, SSKey} {
		if v&val != 0 {
			ret = append(ret, m[val])
		}
	}

	return strings.Join(ret, ",")
}

func enumFromSlurpSource(values []string) (SlurpSource, error) {
	if len(values) == 0 {
		return NoSS, fmt.Errorf("no value provided for source option")
	}

	var result SlurpSource
	for _, val := range values {
		switch val {
		case "etherscan":
			result |= SSEtherscan
		case "key":
			result |= SSKey
		default:
			return NoSS, fmt.Errorf("unknown source: %s", val)
		}
	}

	return result, nil
}

// EXISTING_CODE
// EXISTING_CODE
