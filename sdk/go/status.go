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
	"encoding/json"
	"fmt"
	"io"
	"net/url"
	"strings"

	status "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type StatusOptions struct {
	Modes       StatusModes `json:"modes,omitempty"`
	Diagnose    bool        `json:"diagnose,omitempty"`
	FirstRecord uint64      `json:"firstRecord,omitempty"`
	MaxRecords  uint64      `json:"maxRecords,omitempty"`
	Chains      bool        `json:"chains,omitempty"`
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// String implements the stringer interface
func (opts *StatusOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Status implements the chifra status command for the SDK.
func (opts *StatusOptions) Status(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	if opts.Modes != NoSM {
		values.Set("modes", opts.Modes.String())
	}
	if opts.Diagnose {
		values.Set("diagnose", "true")
	}
	if opts.FirstRecord != 0 {
		values.Set("firstRecord", fmt.Sprintf("%d", opts.FirstRecord))
	}
	if opts.MaxRecords != 0 {
		values.Set("maxRecords", fmt.Sprintf("%d", opts.MaxRecords))
	}
	if opts.Chains {
		values.Set("chains", "true")
	}
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return status.Status(w, values)
}

// statusParseFunc handles specail cases such as structs and enums (if any).
func statusParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	// EXISTING_CODE
	opts, ok := target.(*StatusOptions)
	if !ok {
		return false, fmt.Errorf("parseFunc(chunks): target is not of correct type")
	}

	switch key {
	case "mode":
		var err error
		values := strings.Split(value, ",")
		if opts.Modes, err = enumsFromStrsStatus(values); err != nil {
			return false, err
		} else {
			found = true
		}
	}

	// EXISTING_CODE
	return found, nil
}

// GetStatusOptions returns a filled-in options instance given a string array of arguments.
func GetStatusOptions(args []string) (*StatusOptions, error) {
	var opts StatusOptions
	if err := assignValuesFromArgs(args, statusParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return &opts, nil
}

type StatusModes int

const (
	NoSM StatusModes = 0
	SMIndex = 1 << iota
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
	SMSome = SMIndex | SMBlooms
	SMAll = SMIndex | SMBlooms | SMBlocks | SMTransactions | SMTraces | SMLogs | SMStatements | SMResults | SMState | SMTokens | SMMonitors | SMNames | SMAbis | SMSlurps | SMStaging | SMUnripe | SMMaps
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
		SMIndex: "index",
		SMBlooms: "blooms",
		SMBlocks: "blocks",
		SMTransactions: "transactions",
		SMTraces: "traces",
		SMLogs: "logs",
		SMStatements: "statements",
		SMResults: "results",
		SMState: "state",
		SMTokens: "tokens",
		SMMonitors: "monitors",
		SMNames: "names",
		SMAbis: "abis",
		SMSlurps: "slurps",
		SMStaging: "staging",
		SMUnripe: "unripe",
		SMMaps: "maps",
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
func enumsFromStrsStatus(values []string) (StatusModes, error) {
	if len(values) == 0 {
		return NoSM, fmt.Errorf("no value provided for parts option")
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
