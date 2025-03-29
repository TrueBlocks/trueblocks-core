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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	sdk "github.com/TrueBlocks/trueblocks-sdk/v5"
)

// EXISTING_CODE

// DoInit tests the Init sdk function
func DoInit() {
	file.EstablishFolder("sdkFuzzer-output/init")
	opts := sdk.InitOptions{}
	ShowHeader("DoInit", opts)

	globs := noCache(noEther(globals))
	// firstBlock is a <blknum> --other
	// publisher is not fuzzed
	// sleep is not fuzzed
	// Fuzz Loop
	// EXISTING_CODE
	_ = globs
	// init,command,verbose|version|noop|noColor|chain|
	// opts := sdk.InitOptions{}

	// Publisher  base.Address `json:"publisher,omitempty"`
	// FirstBlock base.Blknum  `json:"firstBlock,omitempty"`
	// Sleep      float64      `json:"sleep,omitempty"`
	// func (opts *InitOptions) InitAll() ([]bool, *types.MetaData, error) {
	// func (opts *InitOptions) InitExample(val string) ([]bool, *types.MetaData, error) {
	// func (opts *InitOptions) InitDryRun() ([]bool, *types.MetaData, error) {

	// if init, _, err := opts.InitAll(); err != nil {
	// 	logger.Error(err)
	// } else {
	// 	if err := SaveAndClean("sdkFuzzer-output/init.json", init, &opts, func() error {
	// 		_, _, err := opts.InitAll()
	// 		return err
	// 	}); err != nil {
	// 		logger.Error(err)
	// 	}
	// }
	// EXISTING_CODE
	Wait()
}

func TestInit(which, value, fn string, opts *sdk.InitOptions) {
	fn = strings.Replace(fn, ".json", "-"+which+".json", 1)
	// EXISTING_CODE
	_ = fn // silence warning
	// EXISTING_CODE

	switch which {
	case "all":
		if all, _, err := opts.InitAll(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile(fn, all); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "example":
		if example, _, err := opts.InitExample(value); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile(fn, example); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "dryrun":
		if dryrun, _, err := opts.InitDryRun(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile(fn, dryrun); err != nil {
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
