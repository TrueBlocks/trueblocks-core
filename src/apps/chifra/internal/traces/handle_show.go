// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tracesPkg

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

func (opts *TracesOptions) HandleShow() error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	nErrors := 0

	abiCache := articulate.NewAbiCache(opts.Conn, opts.Articulate)
	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawTrace], errorChan chan error) {
		if sliceOfMaps, cnt, err := identifiers.AsSliceOfMaps[types.SimpleTransaction](chain, opts.TransactionIds); err != nil {
			errorChan <- err
			cancel()

		} else if cnt == 0 {
			errorChan <- fmt.Errorf("no transactions found")
			cancel()

		} else {
			bar := logger.NewBar(logger.BarOptions{
				Enabled: !testMode && !utils.IsTerminal(),
				Total:   int64(cnt),
			})

			for _, thisMap := range sliceOfMaps {
				thisMap := thisMap
				for app := range thisMap {
					thisMap[app] = new(types.SimpleTransaction)
				}

				iterFunc := func(app types.SimpleAppearance, value *types.SimpleTransaction) error {
					if tx, err := opts.Conn.GetTransactionByAppearance(&app, true); err != nil {
						errorChan <- fmt.Errorf("transaction at %s returned an error: %w", app.Orig(), err)
						return nil

					} else if tx == nil || len(tx.Traces) == 0 {
						errorChan <- fmt.Errorf("transaction at %s has no traces", app.Orig())
						return nil

					} else {
						for index := range tx.Traces {
							if opts.Articulate {
								if err = abiCache.ArticulateTrace(&tx.Traces[index]); err != nil {
									errorChan <- err // continue even with an error
								}
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

				items := make([]types.SimpleTrace, 0, len(thisMap))
				for _, receipt := range thisMap {
					items = append(items, receipt.Traces...)
				}
				sort.Slice(items, func(i, j int) bool {
					if items[i].BlockNumber == items[j].BlockNumber {
						if items[i].TransactionIndex == items[j].TransactionIndex {
							return items[i].GetSortString() < items[j].GetSortString()
						}
						return items[i].TransactionIndex < items[j].TransactionIndex
					}
					return items[i].BlockNumber < items[j].BlockNumber
				})

				for _, item := range items {
					item := item
					modelChan <- &item
				}
			}
			bar.Finish(true)
		}
	}

	extra := map[string]interface{}{
		"articulate": opts.Articulate,
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
