package transactionsPkg

import (
	"context"
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum"
)

func (opts *TransactionsOptions) HandleUniq() (err error, disp bool) {
	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawAppearance], errorChan chan error) {
		for _, rng := range opts.TransactionIds {
			txIds, err := rng.ResolveTxs(opts.Globals.Chain)
			if err != nil && !errors.Is(err, ethereum.NotFound) {
				errorChan <- err
				cancel()
			}

			for _, app := range txIds {
				ts, err := tslib.FromBnToTs(opts.Globals.Chain, uint64(app.BlockNumber))
				if err != nil {
					errorChan <- err
					continue
				}
				modelChan <- &types.SimpleAppearance{
					// Address:          mon.Address,
					BlockNumber:      app.BlockNumber,
					TransactionIndex: app.TransactionIndex,
					Timestamp:        ts,
					Date:             utils.FormattedDate(ts),
				}
			}
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts()), true
}
