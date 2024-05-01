package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoTraces tests the When sdk function
func DoTraces() {
	logger.Info("DoTraces")

	opts := sdk.TracesOptions{
		TransactionIds: []string{"17100101.1"},
	}

	// TransactionIds []string `json:"transactions,omitempty"`
	// Articulate     bool     `json:"articulate,omitempty"`
	// Filter         string   `json:"filter,omitempty"`
	// Globals

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
			states := noEther(globals)
			for _, g := range states {
				opts.Globals = g
				fn := getFilename(baseFn, &opts.Globals)
				TestTraces(fn, &opts)
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

// if tracesCount, _, err := opts.TracesCount(); err != nil {
// 	logger.Error(err)
// } else {
// 	if err := SaveAndClean[types.TraceCount]("usesSDK/tracesCount.json", tracesCount, &opts, func() error {
// 		_, _, err := opts.TracesCount()
// 		return err
// 	}); err != nil {
// 		logger.Error(err)
// 	}
// }
