package main

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoTraces tests the traces sdk function
func DoTraces() {
	file.EstablishFolder("usesSdk-output/traces")
	opts := sdk.TracesOptions{
		TransactionIds: []string{"17100101.1", "3189962.7"},
	}
	ShowHeader("DoTraces", opts)

	// TransactionIds []string
	// Filter string
	// Articulate bool
	// func Traces() []types.Trace
	// func TracesCount() []types.TraceCount

	filters := []string{""} // , "0x2ed0c4!0x2ed128!!0x8bbb73bcb5d553b5a556358d27625323fd781d37!!"}
	art := []bool{false, true}

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
			globs := globals
			for _, g := range globs {
				opts.Globals = g
				fn := getFilename(baseFn, &opts.Globals)
				TestTraces(fn, &opts)
				TestTracesCount(fn, &opts)
			}
		}
	}
}

func TestTraces(fn string, opts *sdk.TracesOptions) {
	if traces, _, err := opts.Traces(); err != nil {
		ReportError(fn, err)
	} else {
		if err := SaveToFile[types.Trace](fn, traces); err != nil {
			ReportError(fn, err)
		} else {
			ReportOkay(fn)
		}
	}
}

func TestTracesCount(fn string, opts *sdk.TracesOptions) {
	fn = strings.ReplaceAll(fn, ".json", "-count.json")
	if tracesCounts, _, err := opts.TracesCount(); err != nil {
		ReportError(fn, err)
	} else {
		if err := SaveToFile[types.TraceCount](fn, tracesCounts); err != nil {
			ReportError(fn, err)
		} else {
			ReportOkay(fn)
		}
	}
}
