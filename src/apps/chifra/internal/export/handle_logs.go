// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"context"
	"fmt"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ExportOptions) HandleLogs(monitorArray []monitor.Monitor) error {
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
	settings := rpcClient.DefaultRpcOptionsSettings{
		Chain: chain,
		Opts:  opts,
	}
	opts.Conn = settings.DefaultRpcOptions()
	if !opts.Conn.Store.ReadOnly() {
		opts.Conn.LatestBlockTimestamp = opts.Conn.GetBlockTimestamp(chain, nil)
	}

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawLog], errorChan chan error) {
		for _, mon := range monitorArray {
			if items, err := opts.readLogs(monitorArray, &mon, filter, errorChan, abiCache); err != nil {
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
	monitorArray []monitor.Monitor,
	mon *monitor.Monitor,
	filter *monitor.AppearanceFilter,
	errorChan chan error,
	abiCache *articulate.AbiCache,
) ([]*types.SimpleLog, error) {
	if txMap, cnt, err := monitor.ReadAppearancesToMap[types.SimpleTransaction](mon, filter); err != nil {
		errorChan <- err
		return nil, err
	} else if !opts.NoZero || cnt > 0 {
		chain := opts.Globals.Chain
		if err := opts.readTransactions(mon, txMap, false /* readTraces */); err != nil { // calls IterateOverMap
			return nil, err
		}

		// Sort the items back into an ordered array by block number
		items := make([]*types.SimpleLog, 0, len(txMap))
		for _, tx := range txMap {
			if tx.Receipt != nil {
				for _, log := range tx.Receipt.Logs {
					log := log
					if opts.isRelevant(monitorArray, log) {
						if opts.matchesFilter(&log) {
							if opts.Articulate {
								if err := abiCache.ArticulateLog(chain, &log); err != nil {
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

func (opts *ExportOptions) isRelevant(monitorArray []monitor.Monitor, log types.SimpleLog) bool {
	if !opts.Relevant {
		return true
	}

	haystack := ""
	for _, topic := range log.Topics {
		haystack += strings.Replace(topic.Hex(), "0x", "", -1)
	}
	haystack += log.Data
	for _, mon := range monitorArray {
		if strings.Contains(strings.ToLower(haystack), mon.Address.Hex()[2:]) {
			return true
		}
	}
	return false
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
	for _, t := range opts.Topics {
		if t == log.Topics[0].Hex() {
			return true
		}
	}
	return len(opts.Topics) == 0
}

// TODO: Just a reminder that eth_getLogs is way faster if one uses it for multiple blocks and/or addresses and or topics
// TODO: It used to work this way in the C++ code, but we lost that during the port.
