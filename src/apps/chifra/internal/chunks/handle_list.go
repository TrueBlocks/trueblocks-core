package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func (opts *ChunksOptions) HandleList(unused []uint64) error {
	testMode := opts.Globals.TestMode
	if testMode {
		logger.Info("Test mode: list pins not tested")
		return nil
	}

	// _, _ = pinning.ListPins(opts.Globals.Chain)
	return nil
}
