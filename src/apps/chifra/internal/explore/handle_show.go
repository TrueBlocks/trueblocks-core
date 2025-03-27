package explorePkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ExploreOptions) HandleShow(rCtx *output.RenderCtx) error {
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		_ = errorChan
		for _, dest := range opts.Destinations {
			dests := dest.Resolve(opts.Globals.Chain, opts.Google, opts.Dalle, opts.Local)
			for _, d := range dests {
				if !opts.NoOpen && !opts.Globals.TestMode {
					utils.OpenBrowser(d.Url)
				}
				modelChan <- &d
			}
		}
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}
