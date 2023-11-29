package transactionsPkg

import (
	"context"
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *TransactionsOptions) HandleShow() (err error) {
	chain := opts.Globals.Chain
	abiCache := articulate.NewAbiCache(opts.Conn, opts.Articulate)
	testMode := opts.Globals.TestMode
	nErrors := 0

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawTransaction], errorChan chan error) {
		var cnt int
		var err error
		var appMap []map[types.SimpleAppearance]*types.SimpleTransaction
		if appMap, cnt, err = identifiers.AsMap2[types.SimpleTransaction](chain, opts.TransactionIds); err != nil {
			errorChan <- err
			cancel()
		} else {
			bar := logger.NewBar(logger.BarOptions{
				Enabled: !opts.Globals.TestMode,
				Total:   int64(cnt),
			})

			for _, thisMap := range appMap {
				thisMap := thisMap
				for app := range thisMap {
					thisMap[app] = new(types.SimpleTransaction)
				}

				iterFunc := func(app types.SimpleAppearance, value *types.SimpleTransaction) error {
					a := &types.RawAppearance{
						BlockNumber:      uint32(app.BlockNumber),
						TransactionIndex: uint32(app.TransactionIndex),
					}
					if tx, err := opts.Conn.GetTransactionByAppearance(a, opts.Traces /* needsTraces */); err != nil {
						return fmt.Errorf("transaction at %s returned an error: %w", app.Orig(), err)
					} else if tx == nil {
						return fmt.Errorf("transaction at %s has no logs", app.Orig())
					} else {
						if opts.Articulate && tx.ArticulatedTx == nil {
							if err = abiCache.ArticulateTransaction(tx); err != nil {
								errorChan <- err // continue even with an error
							}
						}
						*value = *tx
						bar.Tick()
						return nil
					}
				}

				iterErrorChan := make(chan error)
				iterCtx, iterCancel := context.WithCancel(context.Background())
				defer iterCancel()
				go utils.IterateOverMap(iterCtx, iterErrorChan, thisMap, iterFunc)
				for err := range iterErrorChan {
					if !testMode || nErrors == 0 {
						errorChan <- err
						nErrors++
					}
				}

				items := make([]types.SimpleTransaction, 0, len(thisMap))
				for _, tx := range thisMap {
					items = append(items, *tx)
				}
				sort.Slice(items, func(i, j int) bool {
					if items[i].BlockNumber == items[j].BlockNumber {
						return items[i].TransactionIndex < items[j].TransactionIndex
					}
					return items[i].BlockNumber < items[j].BlockNumber
				})
				for _, item := range items {
					item := item
					if !item.BlockHash.IsZero() {
						modelChan <- &item
					}
				}
			}
			bar.Finish(true)
		}
	}

	extra := map[string]interface{}{
		"articulate": opts.Articulate,
		"traces":     opts.Traces,
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
