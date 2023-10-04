package chunksPkg

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"

func (opts *ChunksOptions) HandlePublish(blockNums []uint64) error {
	return validate.Usage("Handler for {0} --publish not yet implemented.", opts.Mode)
}
