package transactionsPkg

import (
	"context"
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/uniq"
	"github.com/ethereum/go-ethereum"
)

func (opts *TransactionsOptions) HandleUniq() (err error) {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.Appearance], errorChan chan error) {
		bar := logger.NewBar(logger.BarOptions{
			Type:    logger.Expanding,
			Enabled: !testMode && !logger.IsTerminal(),
			Total:   250, // estimate since we have no idea how many there are
		})
		procFunc := func(s *types.Appearance) error {
			bar.Tick()
			modelChan <- s
			return nil
		}

		for _, rng := range opts.TransactionIds {
			apps, err := rng.ResolveTxs(chain)
			if err != nil && !errors.Is(err, ethereum.NotFound) {
				errorChan <- err
				cancel()
			}

			for _, app := range apps {
				app := types.Appearance{
					BlockNumber:      app.BlockNumber,
					TransactionIndex: app.TransactionIndex,
				}
				bn := base.Blknum(app.BlockNumber)
				ts := opts.Conn.GetBlockTimestamp(bn)
				addrMap := make(uniq.AddressBooleanMap)
				if trans, err := opts.Conn.GetTransactionByAppearance(&app, true); err != nil {
					errorChan <- err
				} else {
					if err = uniq.GetUniqAddressesInTransaction(chain, procFunc, opts.Flow, trans, ts, addrMap, opts.Conn); err != nil {
						errorChan <- err
					}
				}
			}
		}
		bar.Finish(true /* newLine */)
	}

	extra := map[string]interface{}{
		"uniq": true,
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
