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

// DoTraces tests the Traces sdk function
func DoTraces() {
	file.EstablishFolder("sdkFuzzer-output/traces")
	opts := sdk.TracesOptions{}
	ShowHeader("DoTraces", opts)

	globs := globals
	// EXISTING_CODE
	filters := []string{""} // , "0x2ed0c4!0x2ed128!!0x8bbb73bcb5d553b5a556358d27625323fd781d37!!"}
	art := []bool{false, true}
	opts = sdk.TracesOptions{
		TransactionIds: []string{"17100101.1", "3189962.7"},
	}
	// traces,command,default|caching|ether|raw|
	// TransactionIds []string
	// Filter string
	// Articulate bool
	// func Traces() []types.Trace
	// func TracesCount() []types.TraceCount
	for _, f := range filters {
		for _, a := range art {
			baseFn := "traces/traces"
			if len(f) > 0 {
				baseFn += "-filter"
			}
			if a {
				baseFn += "-articulate"
			}
			opts.Articulate = a
			opts.Filter = f
			for _, g := range globs {
				opts.Globals = g
				fn := getFilename(baseFn, &opts.Globals)
				TestTraces("traces", "", fn, &opts)
				TestTracesCount(fn, &opts)
			}
		}
	}
	// EXISTING_CODE
	Wait()
}

func TestTraces(which, value, fn string, opts *sdk.TracesOptions) {
	fn = strings.Replace(fn, ".json", "-"+which+".json", 1)
	// EXISTING_CODE
	// EXISTING_CODE

	switch which {
	case "traces":
		if traces, _, err := opts.Traces(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Trace](fn, traces); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "count":
		if count, _, err := opts.TracesCount(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.TraceCount](fn, count); err != nil {
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
func TestTracesCount(fn string, opts *sdk.TracesOptions) {
	fn = strings.ReplaceAll(fn, ".json", "-count.json")
	if tracesCounts, _, err := opts.TracesCount(); err != nil {
		ReportError(fn, opts, err)
	} else {
		if err := SaveToFile[types.TraceCount](fn, tracesCounts); err != nil {
			ReportError2(fn, err)
		} else {
			ReportOkay(fn)
		}
	}
}

// EXISTING_CODE
