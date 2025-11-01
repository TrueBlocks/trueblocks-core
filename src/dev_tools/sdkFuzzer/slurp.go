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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/logger"
	sdk "github.com/TrueBlocks/trueblocks-sdk/v6"
)

// EXISTING_CODE

// DoSlurp tests the Slurp sdk function
func DoSlurp() {
	file.EstablishFolder("sdkFuzzer-output/slurp")
	opts := sdk.SlurpOptions{}
	ShowHeader("DoSlurp", opts)

	globs := globals
	// Option 'parts.list<enum>' is an emum
	articulate := []bool{false, true}
	// Option 'source.enum' is an emum
	// blocks is not fuzzed
	// page is not fuzzed
	// pageId is not fuzzed
	// perPage is not fuzzed
	// sleep is not fuzzed
	// Fuzz Loop
	// EXISTING_CODE
	_ = globs
	globs = noCache(globs)
	types := []sdk.SlurpParts{
		sdk.NoSLP,
		sdk.SPExt,
		sdk.SPInt,
		sdk.SPToken,
		sdk.SPNfts,
		sdk.SP1155,
		sdk.SPMiner,
		sdk.SPUncles,
		sdk.SPWithdrawals,
		sdk.SLPSome,
		sdk.SLPAll,
	}
	sources := []sdk.SlurpSource{
		sdk.SSEtherscan,
		sdk.SSCovalent,
		sdk.SSAlchemy,
	}
	// slurp,command,default|caching|ether|
	// Page       uint64      `json:"page,omitempty"`
	// PageId     string      `json:"pageId,omitempty"`
	// PerPage    uint64      `json:"perPage,omitempty"`
	opts = sdk.SlurpOptions{
		Addrs:   []string{fuzzAddresses[0]},
		PerPage: 10,
	}
	for _, s := range sources {
		for _, p := range types {
			if sdk.NoSLP != p && sdk.SSEtherscan != s {
				continue
			}
			for _, art := range articulate {
				baseFn := "slurp/slurp" + "-" + p.String() + "-" + s.String()
				if art {
					baseFn += "-articulate"
				}
				opts.Source = s
				opts.Parts = p
				opts.Articulate = art
				for _, g := range globs {
					opts.Globals = g
					fn := getFilename(baseFn, &opts.Globals)
					TestSlurp("slurp", "", fn, &opts)
					fn = getFilename(baseFn+"-apps", &opts.Globals)
					TestSlurp("appearances", "", fn, &opts)
					fn = getFilename(baseFn+"-count", &opts.Globals)
					TestSlurp("count", "", fn, &opts)
				}
			}
		}
	}
	// EXISTING_CODE
	Wait()
}

func TestSlurp(which, value, fn string, opts *sdk.SlurpOptions) {
	fn = strings.Replace(fn, ".json", "-"+which+".json", 1)
	// EXISTING_CODE
	// EXISTING_CODE

	switch which {
	case "slurp":
		if slurp, _, err := opts.Slurp(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile(fn, slurp); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "appearances":
		if appearances, _, err := opts.SlurpAppearances(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile(fn, appearances); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "count":
		if count, _, err := opts.SlurpCount(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile(fn, count); err != nil {
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
