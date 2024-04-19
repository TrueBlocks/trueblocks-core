package slurpPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	providerPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/provider"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *SlurpOptions) HandleAppearances() error {
	provider, err := opts.Provider()
	if err != nil {
		logger.Fatal(err)
	}
	provider.SetPrintProgress(!opts.Globals.TestMode && !utils.IsTerminal())
	query := &providerPkg.Query{
		Addresses: opts.Addresses(),
		Resources: opts.Types,
	}

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawAppearance], errorChan chan error) {
		appearancesChan := provider.Appearances(ctx, query, errorChan)
		for appearance := range appearancesChan {
			modelChan <- &appearance
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
