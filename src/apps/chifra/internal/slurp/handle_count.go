package slurpPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
)

func (opts *SlurpOptions) HandleCount(rCtx *output.RenderCtx) error {
	provider, err := opts.Provider()
	if err != nil {
		return err
	}
	provider.SetPrintProgress(opts.Globals.ShowProgressNotTesting())

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		monitorChan := provider.Count(rCtx.Ctx, opts.Query(), errorChan)
		for monitor := range monitorChan {
			modelChan <- &monitor
		}
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}

// const maxTestingBlock = 17000000
