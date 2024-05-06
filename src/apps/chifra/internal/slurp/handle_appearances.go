package slurpPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *SlurpOptions) HandleAppearances() error {
	provider, err := opts.Provider()
	if err != nil {
		return err
	}
	provider.SetPrintProgress(!opts.Globals.TestMode && !utils.IsTerminal())

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawAppearance], errorChan chan error) {
		appearancesChan := provider.Appearances(ctx, opts.Query(), errorChan)
		for appearance := range appearancesChan {
			modelChan <- &appearance
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
