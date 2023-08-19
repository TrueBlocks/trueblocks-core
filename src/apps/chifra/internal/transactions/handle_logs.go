// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package transactionsPkg

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

func (opts *TransactionsOptions) HandleLogs() error {
	chain := opts.Globals.Chain
	abiCache := articulate.NewAbiCache(chain, opts.Articulate)
	testMode := opts.Globals.TestMode
	nErrors := 0

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

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawLog], errorChan chan error) {

		var err error
		var txMap map[identifiers.ResolvedId]*types.SimpleTransaction

		if txMap, _, err = identifiers.AsMap[types.SimpleTransaction](chain, opts.TransactionIds); err != nil {
			errorChan <- err
			cancel()
		}

		bar := logger.NewBar(logger.BarOptions{
			Enabled: !opts.Globals.TestMode && len(opts.Globals.File) == 0,
			Total:   int64(len(txMap)),
		})

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
		go utils.IterateOverMap(iterCtx, iterErrorChan, txMap, iterFunc)
		for err := range iterErrorChan {
			iterCancel()
			if !testMode || nErrors == 0 {
				errorChan <- err
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
			if !item.BlockHash.IsZero() {
				for _, log := range item.Receipt.Logs {
					log := log
					if logFilter.PassesFilter(&log) {
						modelChan <- &log
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
