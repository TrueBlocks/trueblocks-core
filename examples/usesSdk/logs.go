package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoLogs tests the When sdk function
func DoLogs() {
	logger.Info("DoLogs")

	opts := sdk.LogsOptions{
		TransactionIds: append(firsts, []string{"17100101.1"}...),
	}

	// TransactionIds []string `json:"transactions,omitempty"`
	// Emitter        []string `json:"emitter,omitempty"`
	// Topic          []string `json:"topic,omitempty"`
	// Articulate     bool     `json:"articulate,omitempty"`

	fns := map[bool]string{
		false: "logs",
		true:  "logs-articulated",
	}

	art := []bool{false, true}
	for _, a := range art {
		opts.Articulate = a
		states := noRaw(noEther(globals))
		for _, g := range states {
			opts.Globals = g
			fn := getFilename(fns[opts.Articulate], &opts.Globals)
			TestLogs(fn, &opts)
		}
	}
}

func TestLogs(fn string, opts *sdk.LogsOptions) {
	Report(fn) //, opts)
	if logs, _, err := opts.Logs(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Log](fn, logs); err != nil {
			logger.Error(err)
		} else {
			logger.Info(colors.Green, "Ok", colors.Off)
		}
	}
	logger.Info()
}
