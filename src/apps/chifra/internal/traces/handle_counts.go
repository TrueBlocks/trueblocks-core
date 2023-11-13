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

func (opts *TracesOptions) HandleCounts() error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	nErrors := 0

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		var err error
		var txMap map[identifiers.ResolvedId]*types.SimpleTransaction
		if txMap, _, err = identifiers.AsMap[types.SimpleTransaction](chain, opts.TransactionIds); err != nil {
			errorChan <- err
			cancel()
		}

		bar := logger.NewBar(logger.BarOptions{
			Enabled: !opts.Globals.TestMode,
			Total:   int64(len(txMap)),
		})

		iterCtx, iterCancel := context.WithCancel(context.Background())
		defer iterCancel()

		iterFunc := func(app identifiers.ResolvedId, value *types.SimpleTransaction) error {
			a := &types.RawAppearance{
				BlockNumber:      uint32(app.BlockNumber),
				TransactionIndex: uint32(app.TransactionIndex),
			}

			if tx, err := opts.Conn.GetTransactionByAppearance(a, true); err != nil {
				errorChan <- fmt.Errorf("transaction at %s returned an error: %w", app.String(), err)
				return nil

			} else if tx == nil || len(tx.Traces) == 0 {
				errorChan <- fmt.Errorf("transaction at %s has no traces", app.String())
				return nil

			} else {
				*value = *tx
				bar.Tick()
				return nil
			}
		}

		iterErrorChan := make(chan error)
		go utils.IterateOverMap(iterCtx, iterErrorChan, txMap, iterFunc)
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

		items := make([]*types.SimpleTransaction, 0, len(txMap))
		for _, tx := range txMap {
			items = append(items, tx)
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
				counter := simpleTraceCount{
					BlockNumber:      uint64(item.BlockNumber),
					TransactionIndex: uint64(item.TransactionIndex),
					TransactionHash:  item.Hash,
					Timestamp:        item.Timestamp,
					TracesCnt:        uint64(len(item.Traces)),
				}
				modelChan <- &counter
			}
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
