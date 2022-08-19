package chunksPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index/bloom"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinning"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ChunksOptions) pinFile(ctx *WalkContext, path string, first bool) (bool, error) {
	type Result struct {
		Range   cache.FileRange         `json:"range,omitempty"`
		Local   types.SimpleChunkRecord `json:"local,omitempty"`
		Remote  types.SimpleChunkRecord `json:"remote,omitempty"`
		Matches bool                    `json:"matches,omitempty"`
		err     error
	}

	rng, _ := cache.RangeFromFilename(path)
	result := Result{
		Range:  rng,
		Local:  types.SimpleChunkRecord{Range: rng.String()},
		Remote: types.SimpleChunkRecord{Range: rng.String()},
	}

	localService, _ := pinning.NewPinningService(opts.Globals.Chain, pinning.Local)
	remoteService, _ := pinning.NewPinningService(opts.Globals.Chain, pinning.Pinata)

	bloomFile := bloom.ToBloomPath(path)
	indexFile := index.ToIndexPath(path)

	if pinning.LocalDaemonRunning() {
		if result.Local.BloomHash, result.err = localService.PinFile(bloomFile, true); result.err != nil {
			return false, result.err
		}
		if result.Local.IndexHash, result.err = localService.PinFile(indexFile, true); result.err != nil {
			return false, result.err
		}
	}

	if opts.Remote {
		if result.Remote.BloomHash, result.err = remoteService.PinFile(bloomFile, false); result.err != nil {
			return false, result.err
		}
		if result.Remote.IndexHash, result.err = remoteService.PinFile(indexFile, false); result.err != nil {
			return false, result.err
		}
	}

	if pinning.LocalDaemonRunning() && opts.Remote {
		result.Matches = result.Local.IndexHash == result.Remote.IndexHash && result.Local.BloomHash == result.Remote.BloomHash
	} else {
		result.Matches = true
	}

	if ctx.Data != nil {
		count, castOk := ctx.Data.(*int)
		if !castOk {
			return true, fmt.Errorf("could not case pinList")
		}
		*count = *count + 1
		if pinning.LocalDaemonRunning() {
			return *count < 2, opts.Globals.RenderObject(result.Local, *count == 1)
		} else {
			return *count < 2, opts.Globals.RenderObject(result.Remote, *count == 1)
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
