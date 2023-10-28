package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinning"
)

func (opts *ChunksOptions) HandleUnpin(unused []uint64) error {
	testMode := opts.Globals.TestMode
	if testMode {
		logger.Info("Test mode: unpin not tested")
		return nil
	}

	if opts.Sleep == 0.0 {
		opts.Sleep = 1.0
	}
	_ = pinning.Unpin(opts.Globals.Chain, opts.Count, opts.Sleep)

	return nil
}
