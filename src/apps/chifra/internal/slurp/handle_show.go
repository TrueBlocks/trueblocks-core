package slurpPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *SlurpOptions) HandleShow(rCtx *output.RenderCtx) error {
	abiCache := articulate.NewAbiCache(opts.Conn, opts.Articulate)

	provider, err := opts.Provider()
	if err != nil {
		return err
	}
	provider.SetPrintProgress(opts.Globals.ShowProgress())

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		txChan := provider.TransactionsByAddress(rCtx.Ctx, opts.Query(), errorChan)
		for tx := range txChan {
			if opts.Articulate {
				if err := abiCache.ArticulateSlurp(&tx); err != nil {
					errorChan <- err // continue even with an error
				}
			}
			modelChan <- &tx
		}
	}

	extraOpts := map[string]any{
		"articulate": opts.Articulate,
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}
