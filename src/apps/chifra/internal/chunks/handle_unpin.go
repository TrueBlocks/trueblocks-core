package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinning"
)

func (opts *ChunksOptions) HandleUnpin(rCtx *output.RenderCtx, blockNums []base.Blknum) error {
	_ = rCtx
	_ = blockNums
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
