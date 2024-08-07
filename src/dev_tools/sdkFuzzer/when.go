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

	"github.com/TrueBlocks/trueblocks-core/sdk/v3"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// EXISTING_CODE

// DoWhen tests the When sdk function
func DoWhen() {
	file.EstablishFolder("sdkFuzzer-output/when")
	opts := sdk.WhenOptions{}
	ShowHeader("DoWhen", opts)

	globs := noEther(globals)
	repair := []bool{false, true}
	check := []bool{false, true}
	update := []bool{false, true}
	deep := []bool{false, true}
	// blocks is not fuzzed
	// truncate is not fuzzed
	// Fuzz Loop
	// EXISTING_CODE
	_ = repair
	_ = check
	_ = update
	_ = deep
	types := []string{"when", "list", "timestamps", "count"}
	// when,command,default|caching|
	// Truncate base.Blknum `json:"truncate,omitempty"`
	// Repair   bool        `json:"repair,omitempty"`
	// Check    bool        `json:"check,omitempty"`
	// Update   bool        `json:"update,omitempty"`
	// Deep     bool        `json:"deep,omitempty"`
	for _, t := range types {
		opts := sdk.WhenOptions{}
		if t != "list" && t != "when" {
			opts.BlockIds = fuzzBlocks
		}
		ShowHeader("DoWhen-"+t, opts)
		for _, g := range globs {
			baseFn := "when/when"
			opts.Globals = g
			baseFn += "-" + t
			fn := getFilename(baseFn, &opts.Globals)
			TestWhen(t, "", fn, &opts)
		}
	}
	// EXISTING_CODE
	Wait()
}

func TestWhen(which, value, fn string, opts *sdk.WhenOptions) {
	fn = strings.Replace(fn, ".json", "-"+which+".json", 1)
	// EXISTING_CODE
	// EXISTING_CODE

	switch which {
	case "when":
		if when, _, err := opts.When(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.NamedBlock](fn, when); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "list":
		if list, _, err := opts.WhenList(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.NamedBlock](fn, list); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "timestamps":
		if timestamps, _, err := opts.WhenTimestamps(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Timestamp](fn, timestamps); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "count":
		if count, _, err := opts.WhenCount(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Count](fn, count); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	default:
		ReportError(fn, opts, fmt.Errorf("unknown which: %s", which))
		logger.Fatal("Quitting...")
		return
	}
}

// EXISTING_CODE
// EXISTING_CODE
