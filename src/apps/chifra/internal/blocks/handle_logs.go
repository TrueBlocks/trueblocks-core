// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package blocksPkg

import (
	"context"
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *BlocksOptions) HandleLogs() error {
	chain := opts.Globals.Chain
	abiCache := articulate.NewAbiCache(opts.Conn, opts.Articulate)
	emitters := []base.Address{}
	for _, e := range opts.Emitter {
		emitters = append(emitters, base.HexToAddress(e))
	}
	topics := []base.Hash{}
	for _, t := range opts.Topic {
		topics = append(topics, base.HexToHash(t))
	}
	logFilter := types.SimpleLogFilter{
		Emitters: emitters,
		Topics:   topics,
	}

	nErrors := 0
	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawLog], errorChan chan error) {
		var err error
		var appMap map[types.SimpleAppearance]*types.SimpleTransaction
		appMap, _, err = identifiers.AsMap[types.SimpleTransaction](chain, opts.BlockIds)
		if err != nil {
			errorChan <- err
			cancel()
		}

		bar := logger.NewBar(logger.BarOptions{
			Enabled: !opts.Globals.TestMode,
			Total:   int64(len(appMap)),
		})

		iterCtx, iterCancel := context.WithCancel(context.Background())
		defer iterCancel()

		iterFunc := func(app types.SimpleAppearance, value *types.SimpleTransaction) error {
			if value.Receipt == nil {
				value.Receipt = &types.SimpleReceipt{}
			}

			bn := uint64(app.BlockNumber)
			ts := opts.Conn.GetBlockTimestamp(bn)
			if logs, err := opts.Conn.GetLogsByNumber(bn, ts); err != nil {
				errorChan <- fmt.Errorf("block at %d returned an error: %w", bn, err)
				return nil

			} else if len(logs) == 0 {
				errorChan <- fmt.Errorf("block at %d has no logs", bn)
				return nil

			} else {
				l := make([]types.SimpleLog, 0, len(logs))
				for index := range logs {
					if opts.Articulate {
						if err = abiCache.ArticulateLog(&logs[index]); err != nil {
							errorChan <- err // continue even with an error
						}
					}
					l = append(l, logs[index])
				}
				bar.Tick()
				value.Receipt.Logs = append(value.Receipt.Logs, l...)
			}
			return nil
		}

		iterErrorChan := make(chan error)
		go utils.IterateOverMap(iterCtx, iterErrorChan, appMap, iterFunc)
		for err := range iterErrorChan {
			// TODO: I don't really want to quit looping here. Just report the error and keep going.
			// iterCancel()
			if !opts.Globals.TestMode || nErrors == 0 {
				errorChan <- err
				// Reporting more than one error causes tests to fail because they
				// appear concurrently so sort differently
				nErrors++
			}
		}
		bar.Finish(true)

		items := make([]types.SimpleLog, 0, len(appMap))
		for _, tx := range appMap {
			tx := tx
			items = append(items, tx.Receipt.Logs...)
		}
		sort.Slice(items, func(i, j int) bool {
			if items[i].BlockNumber == items[j].BlockNumber {
				if items[i].TransactionIndex == items[j].TransactionIndex {
					return items[i].LogIndex < items[j].LogIndex
				}
				return items[i].TransactionIndex < items[j].TransactionIndex
			}
			return items[i].BlockNumber < items[j].BlockNumber
		})

		for _, item := range items {
			item := item
			if !logFilter.PassesFilter(&item) {
				continue
			}
			modelChan <- &item
		}
	}

	extra := map[string]interface{}{
		"articulate": opts.Articulate,
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
