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

	status "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type StatusOptions struct {
	Modes       StatusModes
	Diagnose    bool
	FirstRecord uint64
	MaxRecords  uint64
	Chains      bool
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// Status implements the chifra status command for the SDK.
func (opts *StatusOptions) Status(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	// EXISTING_CODE

	return status.Status(w, values)
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

