package chunksPkg

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"

func (opts *ChunksOptions) HandlePin(blockNums []uint64) error {
	return validate.Usage("Extractor for {0} --remote not yet implemented.", opts.Mode)
}
