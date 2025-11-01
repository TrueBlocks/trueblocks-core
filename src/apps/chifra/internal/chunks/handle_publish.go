package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/validate"
)

func (opts *ChunksOptions) HandlePublish(rCtx *output.RenderCtx, blockNums []base.Blknum) error {
	_ = rCtx
	_ = blockNums
	return validate.Usage("Handler for {0} --publish not yet implemented.", opts.Mode)
}
