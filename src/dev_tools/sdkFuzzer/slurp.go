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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// EXISTING_CODE

// DoSlurp tests the Slurp sdk function
func DoSlurp() {
	file.EstablishFolder("sdkFuzzer-output/slurp")
	opts := sdk.SlurpOptions{}
	ShowHeader("DoSlurp", opts)

	globs := globals
	// Option 'types1' is an enum
	// Option 'source' is an enum
	// EXISTING_CODE
	types1 := []sdk.SlurpTypes{
		sdk.NoST,
		sdk.STExt,
		sdk.STInt,
		sdk.STToken,
		sdk.STNfts,
		sdk.ST1155,
		sdk.STMiner,
		sdk.STUncles,
		sdk.STWithdrawals,
		sdk.STSome,
		sdk.STAll,
	}
	sources := []sdk.SlurpSource{
		sdk.SSEtherscan,
		sdk.SSKey,
		sdk.SSCovalent,
		sdk.SSAlchemy,
	}
	art := []bool{false, true}
	// slurp,command,default|caching|ether|raw|
	// Page       uint64      `json:"page,omitempty"`
	// PageId     string      `json:"pageId,omitempty"`
	// PerPage    uint64      `json:"perPage,omitempty"`
	opts = sdk.SlurpOptions{
		Addrs:   []string{testAddrs[0]},
		PerPage: 10,
	}
	for _, s := range sources {
		for _, t := range types1 {
			if sdk.NoST != t && sdk.SSEtherscan != s {
				continue
			}
			for _, a := range art {
				baseFn := "slurp/slurp" + "-" + t.String() + "-" + s.String()
				if a {
					baseFn += "-articulate"
				}
				opts.Source = s
				opts.Types = t
				opts.Articulate = a
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
			if err := SaveToFile[types.Slurp](fn, slurp); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "appearances":
		if appearances, _, err := opts.SlurpAppearances(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Appearance](fn, appearances); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "count":
		if count, _, err := opts.SlurpCount(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.SlurpCount](fn, count); err != nil {
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
