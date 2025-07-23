package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinning"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func (opts *ChunksOptions) HandleCounts(rCtx *output.RenderCtx, blockNums []base.Blknum) error {
	_ = blockNums
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		_ = errorChan
		var counter types.Count
		vFunc := func(path string, vP any) (bool, error) {
			_ = vP
			isBloom := path == index.ToBloomPath(path)
			isIndex := path == index.ToIndexPath(path)
			if isBloom || isIndex {
				counter.Count++
			}
			return true, nil
		}

		switch opts.Mode {
		case "blooms":
			path := walk.GetRootPathFromCacheType(chain, walk.Index_Bloom)
			_ = walk.ForEveryFileInFolder(path, vFunc, nil)
		case "index", "stats":
			path := walk.GetRootPathFromCacheType(chain, walk.Index_Final)
			_ = walk.ForEveryFileInFolder(path, vFunc, nil)
		case "manifest":
			source := manifest.LocalCache
			if opts.Remote {
				source = manifest.TempContract // don't modify the local manifest when counting
			}
			man, _ := manifest.LoadManifest(chain, opts.PublisherAddr, source)
			counter.Count = uint64(len(man.Chunks))
		case "pins":
			counter.Count = pinning.CountPins(chain, "pinned")
		}

		if testMode {
			counter.Count = 100
		}

		modelChan <- &counter
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}
