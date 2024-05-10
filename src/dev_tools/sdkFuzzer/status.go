// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
package main

// EXISTING_CODE
import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// EXISTING_CODE

// DoStatus tests the Status sdk function
func DoStatus() {
	file.EstablishFolder("sdkFuzzer-output/status")
	opts := sdk.StatusOptions{}
	ShowHeader("DoStatus", opts)

	// FuzzerInits tag

	// EXISTING_CODE
	// func (opts *StatusOptions) StatusDiagnose() ([]bool, *types.MetaData, error) {

	firsts := []uint64{0, 10}
	maxes := []uint64{0, 500}
	chains := []bool{false, true}
	globs := noEther(noRaw(noCache(globals)))
	// status,command,default|
	for _, c := range chains {
		for _, f := range firsts {
			for _, m := range maxes {
				for _, g := range globs {
					baseName := "status/status"
					opts.Chains = c
					if c {
						baseName += "-chains"
					}
					if f > 0 {
						baseName += fmt.Sprintf("-first-%d", f)
					}
					if m > 0 {
						baseName += fmt.Sprintf("-max-%d", m)
					}
					opts.FirstRecord = f
					opts.MaxRecords = m
					opts.Globals = g

					fn := getFilename(baseName, &opts.Globals)
					TestStatus("index", "", fn, &opts)
					TestStatus("blooms", "", fn, &opts)
					TestStatus("blocks", "", fn, &opts)
					TestStatus("transactions", "", fn, &opts)
					TestStatus("traces", "", fn, &opts)
					TestStatus("logs", "", fn, &opts)
					TestStatus("statements", "", fn, &opts)
					TestStatus("results", "", fn, &opts)
					TestStatus("state", "", fn, &opts)
					TestStatus("tokens", "", fn, &opts)
					TestStatus("monitors", "", fn, &opts)
					TestStatus("names", "", fn, &opts)
					TestStatus("abis", "", fn, &opts)
					TestStatus("slurps", "", fn, &opts)
					TestStatus("staging", "", fn, &opts)
					TestStatus("unripe", "", fn, &opts)
					TestStatus("maps", "", fn, &opts)
					TestStatus("some", "", fn, &opts)
					TestStatus("all", "", fn, &opts)
				}
			}
		}
	}
	// EXISTING_CODE
	Wait()
}

func TestStatus(which, value, fn string, opts *sdk.StatusOptions) {
	fn = strings.Replace(fn, ".json", "-"+which+".json", 1)
	// EXISTING_CODE
	var f func() ([]types.Status, *types.MetaData, error)
	// EXISTING_CODE

	switch which {
	case "index":
		f = opts.StatusIndex
	case "blooms":
		f = opts.StatusBlooms
	case "blocks":
		f = opts.StatusBlocks
	case "transactions":
		f = opts.StatusTransactions
	case "traces":
		f = opts.StatusTraces
	case "logs":
		f = opts.StatusLogs
	case "statements":
		f = opts.StatusStatements
	case "results":
		f = opts.StatusResults
	case "state":
		f = opts.StatusState
	case "tokens":
		f = opts.StatusTokens
	case "monitors":
		f = opts.StatusMonitors
	case "names":
		f = opts.StatusNames
	case "abis":
		f = opts.StatusAbis
	case "slurps":
		f = opts.StatusSlurps
	case "staging":
		f = opts.StatusStaging
	case "unripe":
		f = opts.StatusUnripe
	case "maps":
		f = opts.StatusMaps
	case "some":
		f = opts.StatusSome
	case "all":
		f = opts.StatusAll
	default:
		ReportError(fn, opts, fmt.Errorf("unknown which: %s", which))
		return
	}

	if status, _, err := f(); err != nil {
		ReportError(fn, opts, err)
	} else {
		if err := SaveToFile[types.Status](fn, status); err != nil {
			ReportError2(fn, err)
		} else {
			ReportOkay(fn)
		}
	}
}

// EXISTING_CODE
// EXISTING_CODE
