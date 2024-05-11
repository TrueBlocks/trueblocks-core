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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// EXISTING_CODE

// DoList tests the List sdk function
func DoList() {
	file.EstablishFolder("sdkFuzzer-output/list")
	opts := sdk.ListOptions{}
	ShowHeader("DoList", opts)

	globs := noCache(noRaw(noEther(globals)))
	// EXISTING_CODE
	types := []string{"list", "count", "bounds"}
	silents := []bool{false, true}
	reverseds := []bool{false, true}
	publishers := []string{"", "0x02f2b09b33fdbd406ead954a31f98bd29a2a3492"}
	// list,command,default|
	for _, t := range types {
		opts := sdk.ListOptions{
			Addrs: []string{testAddrs[0]},
		}
		ShowHeader("DoList-"+t, &opts)
		for _, publisher := range publishers {
			for _, reverse := range reverseds {
				for _, silent := range silents {
					opts = sdk.ListOptions{
						Addrs: []string{testAddrs[0]},
					}
					nzs := []bool{false, true}
					for _, nz := range nzs {
						baseFn := "list/list"
						if nz && t == "count" {
							opts.NoZero = true
							baseFn += "-noZero"
						}
						if silent {
							opts.Silent = silent
							baseFn += "-silent"
						}
						if reverse {
							opts.Reversed = reverse
							baseFn += "-reverse"
						}
						if len(publisher) > 0 {
							opts.Publisher = base.HexToAddress(publisher)
							baseFn += "-publisher"
						}
						for _, g := range globs {
							opts.Globals = g
							fn2 := baseFn + "-" + t
							fn := getFilename(fn2, &opts.Globals)
							TestList(t, "", fn, &opts)
							opts.FirstBlock = 10277683
							opts.LastBlock = 18993209
							fn2 = baseFn + "-" + t + "-blocks"
							fn = getFilename(fn2, &opts.Globals)
							TestList(t, "", fn, &opts)
							if t != "count" {
								opts.FirstRecord = 10
								opts.MaxRecords = 23
								fn2 = baseFn + "-" + t + "-record"
								fn = getFilename(fn2, &opts.Globals)
								TestList(t, "", fn, &opts)
							}
						}
					}
				}
			}
		}
	}
	// EXISTING_CODE
	Wait()
}

func TestList(which, value, fn string, opts *sdk.ListOptions) {
	fn = strings.Replace(fn, ".json", "-"+which+".json", 1)
	// EXISTING_CODE
	// EXISTING_CODE

	switch which {
	case "list":
		if list, _, err := opts.List(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Appearance](fn, list); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "count":
		if count, _, err := opts.ListCount(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.AppearanceCount](fn, count); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "bounds":
		if bounds, _, err := opts.ListBounds(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Bounds](fn, bounds); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	default:
		ReportError(fn, opts, fmt.Errorf("unknown which: %s", which))
		return
	}
}

// EXISTING_CODE
// EXISTING_CODE
