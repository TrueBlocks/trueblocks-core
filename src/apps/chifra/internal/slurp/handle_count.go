package slurpPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	providerPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/provider"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *SlurpOptions) HandleCount() error {
	provider, err := opts.Provider()
	if err != nil {
		return err
	}
	provider.SetPrintProgress(!opts.Globals.TestMode && !logger.IsTerminal())
	query := &providerPkg.Query{
		Addresses: opts.Addresses(),
		Resources: opts.Types,
		PerPage:   uint(opts.PerPage),
		StartPage: uint(opts.Page),
	}

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawMonitor], errorChan chan error) {
		monitorChan := provider.Count(ctx, query, errorChan)
		for monitor := range monitorChan {
			modelChan <- &monitor
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}

// const maxTestingBlock = 17000000
