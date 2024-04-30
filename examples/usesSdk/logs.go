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

	emitters := []string{"", "0x5564886ca2c518d1964e5fcea4f423b41db9f561"}
	topics := []string{"", "0xa6697e974e6a320f454390be03f74955e8978f1a6971ea6730542e37b66179bc"}
	art := []bool{false, true}

	for _, t := range topics {
		for _, e := range emitters {
			for _, a := range art {
				baseFn := "logs/logs"
				if a {
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
				opts.Articulate = a
				states := noRaw(noEther(globals))
				for _, g := range states {
					opts.Globals = g
					fn := getFilename(baseFn, &opts.Globals)
					TestLogs(fn, &opts)
				}
			}
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
