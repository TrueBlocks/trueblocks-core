package chunksPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *ChunksOptions) HandlePublish(blockNums []base.Blknum) error {
	_ = context.TODO()
	return validate.Usage("Handler for {0} --publish not yet implemented.", opts.Mode)
}
