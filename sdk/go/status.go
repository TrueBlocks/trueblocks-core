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
		values.Set("first_record", fmt.Sprint(opts.FirstRecord))
	}
	if opts.MaxRecords != 0 {
		values.Set("max_records", fmt.Sprint(opts.MaxRecords))
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
	NoSM StatusModes = iota
	SMIndex
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
	SMSome
	SMAll
)

func (v StatusModes) String() string {
	return []string{
		"nosm",
		"index",
		"blooms",
		"blocks",
		"transactions",
		"traces",
		"logs",
		"statements",
		"results",
		"state",
		"tokens",
		"monitors",
		"names",
		"abis",
		"slurps",
		"staging",
		"unripe",
		"maps",
		"some",
		"all",
	}[v]
}

// EXISTING_CODE
// EXISTING_CODE

