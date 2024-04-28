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

type StatusOptions struct {
	FirstRecord uint64 `json:"firstRecord,omitempty"`
	MaxRecords  uint64 `json:"maxRecords,omitempty"`
	Chains      bool   `json:"chains,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *StatusOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// StatusDiagnose implements the chifra status --diagnose command.
func (opts *StatusOptions) StatusDiagnose() ([]bool, *types.MetaData, error) {
	in := opts.toInternal()
	in.Diagnose = true
	return queryStatus[bool](in)
}

type StatusModes int

const (
	NoSM    StatusModes = 0
	SMIndex             = 1 << iota
	SMBlooms
	SMBlocks
	SMTransactions
	SMTraces
	SMLogs
	SMStatements
	SMResults
	SMState
	SMTokens
	SMMonitors
	SMNames
	SMAbis
	SMSlurps
	SMStaging
	SMUnripe
	SMMaps
	SMSome = SMIndex | SMBlooms | SMBlocks | SMTransactions
	SMAll  = SMIndex | SMBlooms | SMBlocks | SMTransactions | SMTraces | SMLogs | SMStatements | SMResults | SMState | SMTokens | SMMonitors | SMNames | SMAbis | SMSlurps | SMStaging | SMUnripe | SMMaps
)

func (v StatusModes) String() string {
	switch v {
	case NoSM:
		return "none"
	case SMSome:
		return "some"
	case SMAll:
		return "all"
	}

	var m = map[StatusModes]string{
		SMIndex:        "index",
		SMBlooms:       "blooms",
		SMBlocks:       "blocks",
		SMTransactions: "transactions",
		SMTraces:       "traces",
		SMLogs:         "logs",
		SMStatements:   "statements",
		SMResults:      "results",
		SMState:        "state",
		SMTokens:       "tokens",
		SMMonitors:     "monitors",
		SMNames:        "names",
		SMAbis:         "abis",
		SMSlurps:       "slurps",
		SMStaging:      "staging",
		SMUnripe:       "unripe",
		SMMaps:         "maps",
	}

	var ret []string
	for _, val := range []StatusModes{SMIndex, SMBlooms, SMBlocks, SMTransactions, SMTraces, SMLogs, SMStatements, SMResults, SMState, SMTokens, SMMonitors, SMNames, SMAbis, SMSlurps, SMStaging, SMUnripe, SMMaps} {
		if v&val != 0 {
			ret = append(ret, m[val])
		}
	}

	return strings.Join(ret, ",")
}

func enumFromStatusModes(values []string) (StatusModes, error) {
	if len(values) == 0 {
		return NoSM, fmt.Errorf("no value provided for modes option")
	}

	if len(values) == 1 && values[0] == "all" {
		return SMAll, nil
	} else if len(values) == 1 && values[0] == "some" {
		return SMSome, nil
	}

	var result StatusModes
	for _, val := range values {
		switch val {
		case "index":
			result |= SMIndex
		case "blooms":
			result |= SMBlooms
		case "blocks":
			result |= SMBlocks
		case "transactions":
			result |= SMTransactions
		case "traces":
			result |= SMTraces
		case "logs":
			result |= SMLogs
		case "statements":
			result |= SMStatements
		case "results":
			result |= SMResults
		case "state":
			result |= SMState
		case "tokens":
			result |= SMTokens
		case "monitors":
			result |= SMMonitors
		case "names":
			result |= SMNames
		case "abis":
			result |= SMAbis
		case "slurps":
			result |= SMSlurps
		case "staging":
			result |= SMStaging
		case "unripe":
			result |= SMUnripe
		case "maps":
			result |= SMMaps
		default:
			return NoSM, fmt.Errorf("unknown modes: %s", val)
		}
	}

	return result, nil
}

// EXISTING_CODE
// EXISTING_CODE
