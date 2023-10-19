package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func (opts *ChunksOptions) HandleUnpin(unused []uint64) error {
	testMode := opts.Globals.TestMode
	if testMode {
		logger.Info("Test mode: unpin not tested")
		return nil
	}

	// _ = pinning.Unpin(opts.Globals.Chain)
	return nil
}
