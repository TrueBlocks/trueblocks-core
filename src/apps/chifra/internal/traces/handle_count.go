// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tracesPkg

import (
	"context"
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *TracesOptions) HandleCount() error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	nErrors := 0

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		apps, _, err := identifiers.IdsToApps(chain, opts.TransactionIds)
		if err != nil {
			errorChan <- err
			cancel()
		}

		if sliceOfMaps, cnt, err := types.AsSliceOfMaps[types.Transaction](apps, false); err != nil {
			errorChan <- err
			cancel()

		} else if cnt == 0 {
			errorChan <- fmt.Errorf("no transactions found")
			cancel()

		} else {
			showProgress := opts.Globals.ShowProgress()
			bar := logger.NewBar(logger.BarOptions{
				Enabled: showProgress,
				Total:   int64(cnt),
			})

			for _, thisMap := range sliceOfMaps {
				for app := range thisMap {
					thisMap[app] = new(types.Transaction)
				}

				iterFunc := func(app types.Appearance, value *types.Transaction) error {
					if tx, err := opts.Conn.GetTransactionByAppearance(&app, true); err != nil {
						delete(thisMap, app)
						return fmt.Errorf("transaction at %s returned an error: %w", app.Orig(), err)

					} else if tx == nil || len(tx.Traces) == 0 {
						delete(thisMap, app)
						return fmt.Errorf("transaction at %s has no traces", app.Orig())

					} else {
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

				items := make([]*types.Transaction, 0, len(thisMap))
				for _, tx := range thisMap {
					items = append(items, tx)
				}
				sort.Slice(items, func(i, j int) bool {
					if items[i].BlockNumber == items[j].BlockNumber {
						return items[i].TransactionIndex < items[j].TransactionIndex
					}
					return items[i].BlockNumber < items[j].BlockNumber
				})

				for _, item := range items {
					counter := types.TraceCount{
						BlockNumber:      item.BlockNumber,
						TransactionIndex: item.TransactionIndex,
						TransactionHash:  item.Hash,
						Timestamp:        item.Timestamp,
						TracesCnt:        uint64(len(item.Traces)),
					}
					modelChan <- &counter
				}
			}
			bar.Finish(true /* newLine */)
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
