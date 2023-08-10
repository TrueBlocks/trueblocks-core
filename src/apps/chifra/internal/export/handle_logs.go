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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ExportOptions) HandleLogs(monitorArray []monitor.Monitor) error {
	chain := opts.Globals.Chain
	abiCache := articulate.NewAbiCache(chain, opts.Articulate)
	testMode := opts.Globals.TestMode
	filter := monitor.NewFilter(
		chain,
		opts.Globals.Verbose,
		opts.Reversed,
		!testMode,
		base.BlockRange{First: opts.FirstBlock, Last: opts.LastBlock},
		base.RecordRange{First: opts.FirstRecord, Last: opts.GetMax()},
	)

	addrArray := make([]base.Address, 0, len(monitorArray))
	for _, mon := range monitorArray {
		addrArray = append(addrArray, mon.Address)
	}

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawLog], errorChan chan error) {
		for _, mon := range monitorArray {
			mon := mon
			if items, err := opts.readLogs(addrArray, &mon, filter, errorChan, abiCache); err != nil {
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

func (opts *ExportOptions) readLogs(
	addrArray []base.Address,
	mon *monitor.Monitor,
	filter *monitor.AppearanceFilter,
	errorChan chan error,
	abiCache *articulate.AbiCache,
) ([]*types.SimpleLog, error) {
	if txMap, cnt, err := monitor.ReadAppearancesToMap[types.SimpleTransaction](mon, filter); err != nil {
		errorChan <- err
		return nil, err
	} else if !opts.NoZero || cnt > 0 {
		if err := opts.readTransactions(mon, txMap, false /* readTraces */); err != nil { // calls IterateOverMap
			return nil, err
		}

		// Sort the items back into an ordered array by block number
		items := make([]*types.SimpleLog, 0, len(txMap))
		for _, tx := range txMap {
			if tx.Receipt != nil {
				for _, log := range tx.Receipt.Logs {
					log := log
					if log.ContainsAny(addrArray) {
						if opts.matchesFilter(&log) {
							if opts.Articulate {
								if err := abiCache.ArticulateLog(&log); err != nil {
									errorChan <- fmt.Errorf("error articulating log: %v", err)
								}
							}
							items = append(items, &log)
						}
					}
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

func (opts *ExportOptions) matchesFilter(log *types.SimpleLog) bool {
	return opts.matchesTopic(log) && opts.matchesEmitter(log)
}

func (opts *ExportOptions) matchesEmitter(log *types.SimpleLog) bool {
	for _, e := range opts.Emitter {
		if e == log.Address.Hex() {
			return true
		}
	}
	return len(opts.Emitter) == 0
}

func (opts *ExportOptions) matchesTopic(log *types.SimpleLog) bool {
	if len(log.Topics) == 0 {
		return false
	}

	for _, t := range opts.Topics {
		if t == log.Topics[0].Hex() {
			return true
		}
	}

	return len(opts.Topics) == 0
}

// TODO: Just a reminder that eth_getLogs is way faster if one uses it for multiple blocks and/or addresses and or topics
// TODO: It used to work this way in the C++ code, but we lost that during the port.
