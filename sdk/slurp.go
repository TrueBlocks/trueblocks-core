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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	// EXISTING_CODE
)

type SlurpOptions struct {
	Addrs      []string          `json:"addrs,omitempty"`
	BlockIds   []string          `json:"blocks,omitempty"`
	Parts      SlurpParts        `json:"parts,omitempty"`
	Articulate bool              `json:"articulate,omitempty"`
	Source     SlurpSource       `json:"source,omitempty"`
	Page       uint64            `json:"page,omitempty"`
	PageId     string            `json:"pageId,omitempty"`
	PerPage    uint64            `json:"perPage,omitempty"`
	Sleep      float64           `json:"sleep,omitempty"`
	RenderCtx  *output.RenderCtx `json:"-"`
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
func (opts *SlurpOptions) SlurpCount() ([]types.Monitor, *types.MetaData, error) {
	in := opts.toInternal()
	in.Count = true
	return querySlurp[types.Monitor](in)
}

type SlurpParts int

const (
	NoSLP SlurpParts = 0
	SPExt            = 1 << iota
	SPInt
	SPToken
	SPNfts
	SP1155
	SPMiner
	SPUncles
	SPWithdrawals
	SLPSome = SPExt | SPInt | SPToken | SPNfts
	SLPAll  = SPExt | SPInt | SPToken | SPNfts | SP1155 | SPMiner | SPUncles | SPWithdrawals
)

func (v SlurpParts) String() string {
	switch v {
	case NoSLP:
		return "none"
	case SLPSome:
		return "some"
	case SLPAll:
		return "all"
	}

	var m = map[SlurpParts]string{
		SPExt:         "ext",
		SPInt:         "int",
		SPToken:       "token",
		SPNfts:        "nfts",
		SP1155:        "1155",
		SPMiner:       "miner",
		SPUncles:      "uncles",
		SPWithdrawals: "withdrawals",
	}

	var ret []string
	for _, val := range []SlurpParts{SPExt, SPInt, SPToken, SPNfts, SP1155, SPMiner, SPUncles, SPWithdrawals} {
		if v&val != 0 {
			ret = append(ret, m[val])
		}
	}

	return strings.Join(ret, ",")
}

func enumFromSlurpParts(values []string) (SlurpParts, error) {
	if len(values) == 0 {
		return NoSLP, fmt.Errorf("no value provided for parts option")
	}

	if len(values) == 1 && values[0] == "all" {
		return SLPAll, nil
	} else if len(values) == 1 && values[0] == "some" {
		return SLPSome, nil
	}

	var result SlurpParts
	for _, val := range values {
		switch val {
		case "ext":
			result |= SPExt
		case "int":
			result |= SPInt
		case "token":
			result |= SPToken
		case "nfts":
			result |= SPNfts
		case "1155":
			result |= SP1155
		case "miner":
			result |= SPMiner
		case "uncles":
			result |= SPUncles
		case "withdrawals":
			result |= SPWithdrawals
		default:
			return NoSLP, fmt.Errorf("unknown parts: %s", val)
		}
	}

	return result, nil
}

type SlurpSource int

const (
	NoSS        SlurpSource = 0
	SSEtherscan             = 1 << iota
	SSKey
	SSCovalent
	SSAlchemy
)

func (v SlurpSource) String() string {
	switch v {
	case NoSS:
		return "none"
	}

	var m = map[SlurpSource]string{
		SSEtherscan: "etherscan",
		SSKey:       "key",
		SSCovalent:  "covalent",
		SSAlchemy:   "alchemy",
	}

	var ret []string
	for _, val := range []SlurpSource{SSEtherscan, SSKey, SSCovalent, SSAlchemy} {
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
		case "covalent":
			result |= SSCovalent
		case "alchemy":
			result |= SSAlchemy
		default:
			return NoSS, fmt.Errorf("unknown source: %s", val)
		}
	}

	return result, nil
}

// EXISTING_CODE
// EXISTING_CODE
