package chunksPkg

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"

func (opts *ChunksOptions) HandlePinAndOrPublish(blockNums []uint64) error {
	if opts.Pin {
		return opts.HandlePin(blockNums)
	} else if opts.Publish {
		return opts.HandlePublish(blockNums)
	}
	return validate.Usage("Call to {} must be either {1}.", "HandlePinAndOrPublish", "--pin or --publish")
}
