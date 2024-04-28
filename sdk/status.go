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
	"strings"
	// EXISTING_CODE
)

type StatusOptions struct {
	Modes       StatusModes `json:"modes,omitempty"`
	Diagnose    bool        `json:"diagnose,omitempty"`
	FirstRecord uint64      `json:"firstRecord,omitempty"`
	MaxRecords  uint64      `json:"maxRecords,omitempty"`
	Chains      bool        `json:"chains,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *StatusOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
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

// EXISTING_CODE
// EXISTING_CODE
