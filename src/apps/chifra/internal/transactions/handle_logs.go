// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package transactionsPkg

import (
	"context"
	"errors"
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

func (opts *TransactionsOptions) HandleLogs() error {
	chain := opts.Globals.Chain
	abiCache := articulate.NewAbiCache(chain, opts.Articulate)
	testMode := opts.Globals.TestMode
	nErrors := 0

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawLog], errorChan chan error) {
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
		if opts.Globals.TestMode {
			errorChan <- errors.New("TESTING_ONLY_filter" + fmt.Sprintf("%+v", logFilter))
		}

		if txMap, err := identifiers.AsMap[types.SimpleTransaction](chain, opts.TransactionIds); err != nil {
			errorChan <- err
			cancel()
		} else {
			showProgress := !opts.Globals.TestMode && len(opts.Globals.File) == 0
			var bar = logger.NewBar("", showProgress, int64(len(txMap)))

			iterCtx, iterCancel := context.WithCancel(context.Background())
			defer iterCancel()

			iterFunc := func(app identifiers.ResolvedId, value *types.SimpleTransaction) error {
				a := &types.RawAppearance{
					BlockNumber:      uint32(app.BlockNumber),
					TransactionIndex: uint32(app.TransactionIndex),
				}
				if tx, err := opts.Conn.GetTransactionByAppearance(a, opts.Traces /* needsTraces */); err != nil {
					return fmt.Errorf("transaction at %s returned an error: %w", app.String(), err)
				} else if tx == nil {
					return fmt.Errorf("transaction at %s has no logs", app.String())
				} else {
					if opts.Articulate && tx.ArticulatedTx == nil {
						if err = abiCache.ArticulateTx(chain, tx); err != nil {
							errorChan <- err // continue even with an error
						}
					}
					*value = *tx
					bar.Tick()
					return nil
				}
			}

			iterErrorChan := make(chan error)
			go utils.IterateOverMap(iterCtx, iterErrorChan, txMap, iterFunc)
			for err := range iterErrorChan {
				// TODO: I don't really want to quit looping here. Just report the error and keep going.
				iterCancel()
				if !testMode || nErrors == 0 {
					errorChan <- err
					// Reporting more than one error causes tests to fail because they
					// appear concurrently so sort differently
					nErrors++
				}
			}
			bar.Finish(true)

			items := make([]types.SimpleTransaction, 0, len(txMap))
			for _, tx := range txMap {
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
				if item.BlockNumber != 0 {
					for _, log := range item.Receipt.Logs {
						log := log
						if opts.shouldShow(logFilter, &log) {
							modelChan <- &log
						}
					}
				}
			}
		}
	}

	extra := map[string]interface{}{
		"logs":      opts.Logs,
		"traces":    opts.Traces,
		"addresses": opts.Uniq,
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}

func (opts *TransactionsOptions) shouldShow(filter types.SimpleLogFilter, log *types.SimpleLog) bool {
	foundEmitter := false
	for _, e := range filter.Emitters {
		if e == log.Address {
			foundEmitter = true
			break
		}
	}

	foundTopic := false
	for _, t := range filter.Topics {
		for _, lt := range log.Topics {
			if t == lt {
				foundTopic = true
				break
			}
		}
	}

	passesEmitter := len(filter.Emitters) == 0 || foundEmitter
	passesTopic := len(filter.Topics) == 0 || foundTopic

	return passesEmitter && passesTopic
}
