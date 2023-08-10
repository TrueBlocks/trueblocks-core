package exportPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ExportOptions) matchesFilter(log *types.SimpleLog) bool {
	return opts.matchesTopic(log) && opts.matchesEmitter(log)
}

func (opts *ExportOptions) matchesEmitter(log *types.SimpleLog) bool {
	for _, e := range opts.Emitter {
		if e == log.Address.Hex() {
			return true
		}
	}
	return len(opts.Emitter) == 0
}

func (opts *ExportOptions) matchesTopic(log *types.SimpleLog) bool {
	if len(log.Topics) == 0 {
		return false
	}

	for _, t := range opts.Topics {
		if t == log.Topics[0].Hex() {
			return true
		}
	}

	return len(opts.Topics) == 0
}
