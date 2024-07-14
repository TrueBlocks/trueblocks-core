package slurpPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *SlurpOptions) HandleCount(rCtx output.RenderCtx) error {
	provider, err := opts.Provider()
	if err != nil {
		return err
	}
	provider.SetPrintProgress(opts.Globals.ShowProgressNotTesting())

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		monitorChan := provider.Count(ctx, opts.Query(), errorChan)
		for monitor := range monitorChan {
			modelChan <- &monitor
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}

// const maxTestingBlock = 17000000
