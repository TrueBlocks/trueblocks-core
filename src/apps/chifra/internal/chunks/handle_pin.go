package chunksPkg

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"

func (opts *ChunksOptions) HandlePin(blockNums []uint64) error {
	if opts.Remote {
		return validate.Usage("Extractor for {0} --pin --remote not yet implemented.", opts.Mode)
	}
	return validate.Usage("Extractor for {0} --pin not yet implemented.", opts.Mode)
}
