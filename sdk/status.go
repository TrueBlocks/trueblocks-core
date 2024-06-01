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
func (opts StatusOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// StatusIndex implements the chifra status index command.
func (opts *StatusOptions) StatusIndex() ([]types.Status, *types.MetaData, error) {
	in := opts.toInternal()
	in.Modes = SMIndex
	return queryStatus[types.Status](in)
}

// StatusBlooms implements the chifra status blooms command.
func (opts *StatusOptions) StatusBlooms() ([]types.Status, *types.MetaData, error) {
	in := opts.toInternal()
	in.Modes = SMBlooms
	return queryStatus[types.Status](in)
}

// StatusBlocks implements the chifra status blocks command.
func (opts *StatusOptions) StatusBlocks() ([]types.Status, *types.MetaData, error) {
	in := opts.toInternal()
	in.Modes = SMBlocks
	return queryStatus[types.Status](in)
}

// StatusTransactions implements the chifra status transactions command.
func (opts *StatusOptions) StatusTransactions() ([]types.Status, *types.MetaData, error) {
	in := opts.toInternal()
	in.Modes = SMTransactions
	return queryStatus[types.Status](in)
}

// StatusTraces implements the chifra status traces command.
func (opts *StatusOptions) StatusTraces() ([]types.Status, *types.MetaData, error) {
	in := opts.toInternal()
	in.Modes = SMTraces
	return queryStatus[types.Status](in)
}

// StatusLogs implements the chifra status logs command.
func (opts *StatusOptions) StatusLogs() ([]types.Status, *types.MetaData, error) {
	in := opts.toInternal()
	in.Modes = SMLogs
	return queryStatus[types.Status](in)
}

// StatusStatements implements the chifra status statements command.
func (opts *StatusOptions) StatusStatements() ([]types.Status, *types.MetaData, error) {
	in := opts.toInternal()
	in.Modes = SMStatements
	return queryStatus[types.Status](in)
}

// StatusResults implements the chifra status results command.
func (opts *StatusOptions) StatusResults() ([]types.Status, *types.MetaData, error) {
	in := opts.toInternal()
	in.Modes = SMResults
	return queryStatus[types.Status](in)
}

// StatusState implements the chifra status state command.
func (opts *StatusOptions) StatusState() ([]types.Status, *types.MetaData, error) {
	in := opts.toInternal()
	in.Modes = SMState
	return queryStatus[types.Status](in)
}

// StatusTokens implements the chifra status tokens command.
func (opts *StatusOptions) StatusTokens() ([]types.Status, *types.MetaData, error) {
	in := opts.toInternal()
	in.Modes = SMTokens
	return queryStatus[types.Status](in)
}

// StatusMonitors implements the chifra status monitors command.
func (opts *StatusOptions) StatusMonitors() ([]types.Status, *types.MetaData, error) {
	in := opts.toInternal()
	in.Modes = SMMonitors
	return queryStatus[types.Status](in)
}

// StatusNames implements the chifra status names command.
func (opts *StatusOptions) StatusNames() ([]types.Status, *types.MetaData, error) {
	in := opts.toInternal()
	in.Modes = SMNames
	return queryStatus[types.Status](in)
}

// StatusAbis implements the chifra status abis command.
func (opts *StatusOptions) StatusAbis() ([]types.Status, *types.MetaData, error) {
	in := opts.toInternal()
	in.Modes = SMAbis
	return queryStatus[types.Status](in)
}

// StatusSlurps implements the chifra status slurps command.
func (opts *StatusOptions) StatusSlurps() ([]types.Status, *types.MetaData, error) {
	in := opts.toInternal()
	in.Modes = SMSlurps
	return queryStatus[types.Status](in)
}

// StatusStaging implements the chifra status staging command.
func (opts *StatusOptions) StatusStaging() ([]types.Status, *types.MetaData, error) {
	in := opts.toInternal()
	in.Modes = SMStaging
	return queryStatus[types.Status](in)
}

// StatusUnripe implements the chifra status unripe command.
func (opts *StatusOptions) StatusUnripe() ([]types.Status, *types.MetaData, error) {
	in := opts.toInternal()
	in.Modes = SMUnripe
	return queryStatus[types.Status](in)
}

// StatusMaps implements the chifra status maps command.
func (opts *StatusOptions) StatusMaps() ([]types.Status, *types.MetaData, error) {
	in := opts.toInternal()
	in.Modes = SMMaps
	return queryStatus[types.Status](in)
}

// StatusSome implements the chifra status some command.
func (opts *StatusOptions) StatusSome() ([]types.Status, *types.MetaData, error) {
	in := opts.toInternal()
	in.Modes = SMSome
	return queryStatus[types.Status](in)
}

// StatusAll implements the chifra status all command.
func (opts *StatusOptions) StatusAll() ([]types.Status, *types.MetaData, error) {
	in := opts.toInternal()
	in.Modes = SMAll
	return queryStatus[types.Status](in)
}

// StatusDiagnose implements the chifra status --diagnose command.
func (opts *StatusOptions) StatusDiagnose() ([]types.Status, *types.MetaData, error) {
	in := opts.toInternal()
	in.Diagnose = true
	return queryStatus[types.Status](in)
}

// StatusHealthcheck implements the chifra status --healthcheck command.
func (opts *StatusOptions) StatusHealthcheck() ([]types.Status, *types.MetaData, error) {
	in := opts.toInternal()
	in.Healthcheck = true
	return queryStatus[types.Status](in)
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
// Status implements the chifra status command.
func (opts *StatusOptions) Status() ([]types.Status, *types.MetaData, error) {
	return opts.StatusDiagnose()
}

// EXISTING_CODE
