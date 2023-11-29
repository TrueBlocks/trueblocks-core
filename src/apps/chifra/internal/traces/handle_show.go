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
	abiCache := articulate.NewAbiCache(opts.Conn, opts.Articulate)
	testMode := opts.Globals.TestMode
	nErrors := 0

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawTrace], errorChan chan error) {
		// var cnt int
		var err error
		var appMap map[types.SimpleAppearance]*types.SimpleTransaction
		if appMap, _, err = identifiers.AsMap[types.SimpleTransaction](chain, opts.TransactionIds); err != nil {
			errorChan <- err
			cancel()
		} else {
			bar := logger.NewBar(logger.BarOptions{
				Enabled: !opts.Globals.TestMode,
				Total:   int64(len(appMap)),
			})

			iterFunc := func(app types.SimpleAppearance, value *types.SimpleTransaction) error {
				a := &types.RawAppearance{
					BlockNumber:      uint32(app.BlockNumber),
					TransactionIndex: uint32(app.TransactionIndex),
				}

				if tx, err := opts.Conn.GetTransactionByAppearance(a, true); err != nil {
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
			go utils.IterateOverMap(iterCtx, iterErrorChan, appMap, iterFunc)
			for err := range iterErrorChan {
				// TODO: I don't really want to quit looping here. Just report the error and keep going.
				// iterCancel()
				if !testMode || nErrors == 0 {
					errorChan <- err
					// Reporting more than one error causes tests to fail because they
					// appear concurrently so sort differently
					nErrors++
				}
			}
			bar.Finish(true)

			items := make([]types.SimpleTrace, 0, len(appMap))
			for _, receipt := range appMap {
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
				if !item.BlockHash.IsZero() {
					modelChan <- &item
				}
			}
		}
	}

	extra := map[string]interface{}{
		"articulate": opts.Articulate,
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
