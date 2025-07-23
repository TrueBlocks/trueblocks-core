package types

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

var verbose bool = false

func SetVerbose(v bool) {
	verbose = v
}

func IsVerbose() bool {
	return verbose
}

func VerboseLog(args ...interface{}) {
	if verbose {
		logger.Info(args...)
	}
}
