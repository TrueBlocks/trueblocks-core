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

// DoConfig tests the Config sdk function
func DoConfig() {
	file.EstablishFolder("sdkFuzzer-output/config")
	opts := sdk.ConfigOptions{}
	ShowHeader("DoConfig", opts)

	globs := noCache(noEther(globals))
	// Fuzz Loop
	// EXISTING_CODE
	for _, g := range globs {
		opts.Globals = g
		fn := getFilename("config/paths", &opts.Globals)
		TestConfig("paths", "", fn, &opts)
	}
	// EXISTING_CODE
	Wait()
}

func TestConfig(which, value, fn string, opts *sdk.ConfigOptions) {
	fn = strings.Replace(fn, ".json", "-"+which+".json", 1)
	// EXISTING_CODE
	// EXISTING_CODE

	switch which {
	case "paths":
		if paths, _, err := opts.ConfigPaths(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.CacheItem](fn, paths); err != nil {
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
