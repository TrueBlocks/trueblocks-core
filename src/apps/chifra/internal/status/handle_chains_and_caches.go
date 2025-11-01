package statusPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
)

func (opts *StatusOptions) HandleChainsAndCaches(rCtx *output.RenderCtx, forceStatus bool) error {
	if len(opts.OrigModes) > 0 {
		return opts.HandleModes(rCtx)
	}
	testMode := opts.Globals.TestMode
	chain := opts.Globals.Chain

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		s, err := opts.GetStatus(opts.Diagnose)
		if err != nil {
			errorChan <- err
			return
		}

		if forceStatus || (opts.Chains && opts.Caches) {
			modelChan <- s

		} else if opts.Chains {
			for _, chain := range s.Chains {
				modelChan <- &chain
			}

		} else if opts.Caches {
			for _, cacheItem := range s.Caches {
				modelChan <- &cacheItem
			}
		}
	}

	extraOpts := map[string]any{
		"testMode": testMode,
		"chain":    chain,
		"caches":   opts.Caches,
		"chains":   opts.Chains,
	}
	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}
