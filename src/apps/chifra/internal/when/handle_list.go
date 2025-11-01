package whenPkg

import (
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *WhenOptions) HandleList(rCtx *output.RenderCtx) error {
	chain := opts.Globals.Chain

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		results, err := tslib.GetSpecials(chain)
		if err != nil {
			errorChan <- err
			if errors.Is(err, ethereum.NotFound) {
				return
			}
			rCtx.Cancel()
			return
		}

		for _, result := range results {
			if opts.Globals.Verbose || result.Component == "execution" {
				modelChan <- &result
			}
		}
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}
