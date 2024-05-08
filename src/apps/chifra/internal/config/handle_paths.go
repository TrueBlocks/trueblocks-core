package configPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// HandlePaths handles the paths command for the command line. Returns error only as per cobra.
func (opts *ConfigOptions) HandlePaths() error {
	testMode := opts.Globals.TestMode

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawCacheItem], errorChan chan error) {
		root, cache, index := config.PathToRootConfig(),
			config.PathToCache(opts.Globals.Chain),
			config.PathToIndex(opts.Globals.Chain)
		if testMode {
			root, cache, index = "--path--", "--path--", "--path--"
		}

		paths := []types.CacheItem{
			{
				Path:          root,
				CacheItemType: "configPath",
			},
			{
				Path:          cache,
				CacheItemType: "cachePath",
			},
			{
				Path:          index,
				CacheItemType: "indexPath",
			},
		}
		for _, s := range paths {
			modelChan <- &s
		}
	}

	extra := map[string]interface{}{
		"configPaths": true,
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
