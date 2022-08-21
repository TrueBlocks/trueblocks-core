package chunksPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinning"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ChunksOptions) pinFile(ctx *WalkContext, path string, first bool) (bool, error) {
	result, err := pinning.PinChunk(opts.Globals.Chain, path, opts.Remote)
	if err != nil {
		return false, err
	}

	if !result.Matches {
		logger.Log(logger.Error, "Remote and local pins don't match", result)
	}

	if ctx.Data != nil {
		count, castOk := ctx.Data.(*int)
		if !castOk {
			return true, fmt.Errorf("could not cast count")
		}
		*count = *count + 1
		if pinning.LocalDaemonRunning() {
			return true, opts.Globals.RenderObject(result.Local, *count == 1)
		} else {
			return true, opts.Globals.RenderObject(result.Remote, *count == 1)
		}
	}

	return true, nil
}

func (opts *ChunksOptions) HandlePinIndex(blockNums []uint64) error {
	defer opts.Globals.RenderFooter()
	err := opts.Globals.RenderHeader(types.SimpleChunkRecord{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
	if err != nil {
		return err
	}

	count := int(0)
	ctx := WalkContext{
		VisitFunc: opts.pinFile,
		Data:      &count,
	}

	return opts.WalkIndexFiles(&ctx, cache.Index_Bloom, blockNums)
}
