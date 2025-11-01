package slurpPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
)

func (opts *SlurpOptions) HandleAppearances(rCtx *output.RenderCtx) error {
	provider, err := opts.Provider()
	if err != nil {
		return err
	}
	provider.SetPrintProgress(opts.Globals.ShowProgress())

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		appearancesChan := provider.Appearances(rCtx.Ctx, opts.Query(), errorChan)
		for appearance := range appearancesChan {
			modelChan <- &appearance
		}
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}
