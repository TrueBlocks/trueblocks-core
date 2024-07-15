package slurpPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *SlurpOptions) HandleAppearances(rCtx *output.RenderCtx) error {
	provider, err := opts.Provider()
	if err != nil {
		return err
	}
	provider.SetPrintProgress(opts.Globals.ShowProgress())

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		appearancesChan := provider.Appearances(ctx, opts.Query(), errorChan)
		for appearance := range appearancesChan {
			modelChan <- &appearance
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
