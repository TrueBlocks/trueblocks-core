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

// DoLogs tests the Logs sdk function
func DoLogs() {
	file.EstablishFolder("sdkFuzzer-output/logs")
	opts := sdk.LogsOptions{}
	ShowHeader("DoLogs", opts)

	globs := noEther(globals)
	emitter := fuzzEmitters
	topic := fuzzTopics
	articulate := []bool{false, true}
	// Fuzz Loop
	// EXISTING_CODE
	for _, t := range topic {
		for _, e := range emitter {
			for _, art := range articulate {
				baseFn := "logs/logs"
				opts = sdk.LogsOptions{
					TransactionIds: append(firsts, []string{"17100101.1"}...),
				}
				opts.Articulate = art
				if art {
					baseFn += "-articulate"
				}
				opts.Emitter = []string{e}
				if len(e) > 0 {
					baseFn += "-emitter"
				}
				opts.Topic = []string{t}
				if len(t) > 0 {
					baseFn += "-topic"
				}
				for _, g := range globs {
					opts.Globals = g
					fn := getFilename(baseFn, &opts.Globals)
					TestLogs("logs", "", fn, &opts)
				}
			}
		}
	}
	// EXISTING_CODE
	Wait()
}

func TestLogs(which, value, fn string, opts *sdk.LogsOptions) {
	fn = strings.Replace(fn, ".json", "-"+which+".json", 1)
	// EXISTING_CODE
	// EXISTING_CODE

	switch which {
	case "logs":
		if logs, _, err := opts.Logs(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile(fn, logs); err != nil {
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
