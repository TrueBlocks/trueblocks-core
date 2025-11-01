package chunksPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/pinning"
)

func (opts *ChunksOptions) HandleUnpin(rCtx *output.RenderCtx, blockNums []base.Blknum) error {
	_ = rCtx
	_ = blockNums
	testMode := opts.Globals.TestMode
	if testMode && !opts.DryRun {
		logger.Info("Test mode: unpin not tested")
		return nil
	}

	if opts.Sleep == 0.0 {
		opts.Sleep = 1.0
	}

	if opts.DryRun {
		logger.Info(fmt.Sprintf("DRY RUN: Would unpin %t chunks with %.1f second sleep", opts.Count, opts.Sleep))
		return nil
	}

	_ = pinning.Unpin(opts.Globals.Chain, opts.Count, opts.Sleep)

	return nil
}
