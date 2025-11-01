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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/logger"
	sdk "github.com/TrueBlocks/trueblocks-sdk/v5"
)

// EXISTING_CODE

// DoExplore tests the Explore sdk function
func DoExplore() {
	file.EstablishFolder("sdkFuzzer-output/explore")
	opts := sdk.ExploreOptions{}
	ShowHeader("DoExplore", opts)

	globs := noCache(noEther(globals))
	noOpen := []bool{false, true}
	local := []bool{false, true}
	google := []bool{false, true}
	dalle := []bool{false, true}
	// Fuzz Loop
	// EXISTING_CODE
	_ = globs
	_ = noOpen
	_ = local
	_ = google
	_ = dalle
	// EXISTING_CODE
	Wait()
}

func TestExplore(which, value, fn string, opts *sdk.ExploreOptions) {
	fn = strings.Replace(fn, ".json", "-"+which+".json", 1)
	// EXISTING_CODE
	// EXISTING_CODE

	switch which {
	case "explore":
		if explore, _, err := opts.Explore(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile(fn, explore); err != nil {
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
