package chunksPkg

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"

func (opts *ChunksOptions) HandleTag(blockNums []uint64) error {
	if opts.Globals.TestMode {
		logger.Warn("Tag option not tested.")
		return nil
	}

	return nil
}
