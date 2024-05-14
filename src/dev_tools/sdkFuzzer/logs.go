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

// DoLogs tests the Logs sdk function
func DoLogs() {
	file.EstablishFolder("sdkFuzzer-output/logs")
	opts := sdk.LogsOptions{}
	ShowHeader("DoLogs", opts)

	globs := noEther(globals)
	articulate := []bool{false, true}
	// EXISTING_CODE
	emitters := []string{"", "0x5564886ca2c518d1964e5fcea4f423b41db9f561"}
	topics := []string{"", "0xa6697e974e6a320f454390be03f74955e8978f1a6971ea6730542e37b66179bc"}
	opts = sdk.LogsOptions{
		TransactionIds: append(firsts, []string{"17100101.1"}...),
	}
	// logs,command,default|caching|raw|
	for _, t := range topics {
		for _, e := range emitters {
			for _, art := range articulate {
				baseFn := "logs/logs"
				if art {
					baseFn += "-articulate"
				}
				if len(e) > 0 {
					opts.Emitter = []string{e}
					baseFn += "-emitter"
				}
				if len(t) > 0 {
					opts.Topic = []string{t}
					baseFn += "-topic"
				}
				opts.Articulate = art
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
			if err := SaveToFile[types.Log](fn, logs); err != nil {
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
