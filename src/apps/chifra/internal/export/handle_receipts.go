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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ExportOptions) HandleReceipts(monitorArray []monitor.Monitor) error {
	abiCache := articulate.NewAbiCache()
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	filter := monitor.NewFilter(
		chain,
		opts.Globals.Verbose,
		opts.Reversed,
		!testMode,
		base.BlockRange{First: opts.FirstBlock, Last: opts.LastBlock},
		base.RecordRange{First: opts.FirstRecord, Last: opts.GetMax()},
	)

	// TODO: Why does this have to dirty the caller?
	settings := rpcClient.ConnectionSettings{
		Chain: chain,
		Opts:  opts,
	}
	opts.Conn = settings.DefaultRpcOptions()

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawReceipt], errorChan chan error) {
		for _, mon := range monitorArray {
			if items, err := opts.readReceipts(monitorArray, &mon, filter, errorChan, abiCache); err != nil {
				errorChan <- err
				continue
			} else {
				for _, item := range items {
					item := item
					modelChan <- item
				}
			}
		}
	}

	extra := map[string]interface{}{
		"testMode":   testMode,
		"export":     true,
		"articulate": opts.Articulate,
	}

	if opts.Globals.Verbose || opts.Globals.Format == "json" {
		parts := names.Custom | names.Prefund | names.Regular
		if namesMap, err := names.LoadNamesMap(chain, parts, nil); err != nil {
			return err
		} else {
			extra["namesMap"] = namesMap
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}

func (opts *ExportOptions) readReceipts(
	monitorArray []monitor.Monitor,
	mon *monitor.Monitor,
	filter *monitor.AppearanceFilter,
	errorChan chan error,
	abiCache *articulate.AbiCache,
) ([]*types.SimpleReceipt, error) {
	if txMap, cnt, err := monitor.ReadAppearancesToMap[types.SimpleTransaction](mon, filter); err != nil {
		errorChan <- err
		return nil, err
	} else if !opts.NoZero || cnt > 0 {
		chain := opts.Globals.Chain
		if err := opts.readTransactions(mon, txMap, false /* readTraces */); err != nil { // calls IterateOverMap
			return nil, err
		}

		// Sort the items back into an ordered array by block number
		items := make([]*types.SimpleReceipt, 0, len(txMap))
		for _, tx := range txMap {
			if tx.Receipt == nil {
				continue
			}
			filteredLogs := make([]types.SimpleLog, 0, len(tx.Receipt.Logs))
			for _, log := range tx.Receipt.Logs {
				log := log
				if opts.isRelevant(monitorArray, log) {
					if opts.matchesFilter(&log) {
						if opts.Articulate {
							if err := abiCache.ArticulateLog(chain, &log); err != nil {
								errorChan <- fmt.Errorf("error articulating log: %v", err)
							}
						}
						filteredLogs = append(filteredLogs, log)
					}
				}
			}
			tx.Receipt.Logs = filteredLogs
			items = append(items, tx.Receipt)
		}
		sort.Slice(items, func(i, j int) bool {
			itemI := items[i]
			itemJ := items[j]
			if itemI.BlockNumber == itemJ.BlockNumber {
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
