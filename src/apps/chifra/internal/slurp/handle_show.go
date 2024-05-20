package slurpPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *SlurpOptions) HandleShow() error {
	abiCache := articulate.NewAbiCache(opts.Conn, opts.Articulate)

	provider, err := opts.Provider()
	if err != nil {
		return err
	}
	provider.SetPrintProgress(!opts.Globals.TestMode && !logger.IsTerminal())

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.Slurp], errorChan chan error) {
		txChan := provider.TransactionsByAddress(ctx, opts.Query(), errorChan)
		for tx := range txChan {
			if opts.Articulate {
				if err := abiCache.ArticulateSlurp(&tx); err != nil {
					errorChan <- err // continue even with an error
				}
			}
			modelChan <- &tx
		}
	}

	extra := map[string]interface{}{
		"articulate": opts.Articulate,
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
