// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"context"
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ExportOptions) HandleStatements(monitorArray []monitor.Monitor) error {
	// if opts.Accounting {
	// 	// TODO: BOGUS - RECONSIDER THIS
	// 	opts.Articulate = true
	// }

	abiCache := articulate.NewAbiCache()
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	filter := monitor.NewFilter(
		chain,
		true, // TODO: Should we use Verbose here?
		opts.Reversed,
		!testMode,
		base.BlockRange{First: opts.FirstBlock, Last: opts.LastBlock},
		base.RecordRange{First: opts.FirstRecord, Last: opts.GetMax()},
	)
	rpcOptions := opts.Globals.DefaultRpcOptions(nil)

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawStatement], errorChan chan error) {
		for _, mon := range monitorArray {
			if statements, err := opts.readStatements(monitorArray, &mon, filter, errorChan, abiCache, rpcOptions); err != nil {
				errorChan <- err
			} else {
				for _, statement := range statements {
					statement := statement
					modelChan <- statement
				}
			}
		}
	}

	extra := map[string]interface{}{
		"articulate": opts.Articulate,
		"testMode":   testMode,
		"export":     true,
		"ether":      opts.Globals.Ether,
	}

	if opts.Globals.Verbose || opts.Globals.Format == "json" {
		parts := names.Custom | names.Prefund | names.Regular
		namesMap, err := names.LoadNamesMap(chain, parts, nil)
		if err != nil {
			return err
		}
		extra["namesMap"] = namesMap
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}

func (opts *ExportOptions) readStatements(
	monitorArray []monitor.Monitor,
	mon *monitor.Monitor,
	filter *monitor.AppearanceFilter,
	errorChan chan error,
	abiCache *articulate.AbiCache,
	rpcOptions *rpcClient.Options,
) ([]*types.SimpleStatement, error) {
	if txMap, cnt, err := monitor.ReadAppearancesToMap[types.SimpleTransaction](mon, filter); err != nil {
		errorChan <- err
		return nil, err
	} else if !opts.NoZero || cnt > 0 {
		chain := opts.Globals.Chain
		testMode := opts.Globals.TestMode
		if err := opts.readTransactions(mon, txMap, false /* readTraces */, rpcOptions); err != nil {
			return nil, err
		}

		txArray := make([]*types.SimpleTransaction, 0, len(txMap))
		for _, tx := range txMap {
			txArray = append(txArray, tx)
		}

		sort.Slice(txArray, func(i, j int) bool {
			itemI := txArray[i]
			itemJ := txArray[j]
			if itemI.BlockNumber == itemJ.BlockNumber {
				return itemI.TransactionIndex < itemJ.TransactionIndex
			}
			return itemI.BlockNumber < itemJ.BlockNumber
		})

		// Sort the items back into an ordered array by block number
		items := make([]*types.SimpleStatement, 0, len(txMap))
		ledgers := ledger.NewLedger(
			chain,
			mon.Address,
			opts.FirstBlock,
			opts.LastBlock,
			opts.Globals.Ether,
			testMode,
			opts.NoZero,
			opts.Traces,
			&opts.Asset,
		)
		if opts.Accounting {
			apps := make([]types.SimpleAppearance, 0, len(txMap))
			for _, tx := range txArray {
				apps = append(apps, types.SimpleAppearance{
					BlockNumber:      uint32(tx.BlockNumber),
					TransactionIndex: uint32(tx.TransactionIndex),
				})
			}
			ledgers.SetContexts(chain, apps, filter.GetOuterBounds())
		}

		// we need them sorted for the following to work
		for _, tx := range txArray {
			if opts.Articulate {
				if err := abiCache.ArticulateTx(chain, tx); err != nil {
					errorChan <- fmt.Errorf("error articulating tx: %v", err)
				}
			}

			ledgers.Tx = tx // we need this below
			if stmts := ledgers.GetStatementsFromTransaction(tx); len(stmts) > 0 {
				for _, statement := range stmts {
					statement := statement
					items = append(items, statement)
				}
			}
		}

		sort.Slice(items, func(i, j int) bool {
			itemI := items[i]
			itemJ := items[j]
			if itemI.BlockNumber == itemJ.BlockNumber {
				if itemI.TransactionIndex == itemJ.TransactionIndex {
					return itemI.LogIndex < itemJ.LogIndex
				}
				return itemI.TransactionIndex < itemJ.TransactionIndex
			}
			return itemI.BlockNumber < itemJ.BlockNumber
		})

		// Return the array of items
		return items, nil
	} else {
		errorChan <- fmt.Errorf("no appearances found for %s", mon.Address.Hex())
		return nil, nil
	}
}
