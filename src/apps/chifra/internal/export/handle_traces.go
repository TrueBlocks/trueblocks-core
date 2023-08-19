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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ExportOptions) HandleTraces(monitorArray []monitor.Monitor) error {
	chain := opts.Globals.Chain
	abiCache := articulate.NewAbiCache(chain, opts.Articulate)
	testMode := opts.Globals.TestMode
	filter := filter.NewFilter(
		opts.Reversed,
		base.BlockRange{First: opts.FirstBlock, Last: opts.LastBlock},
		base.RecordRange{First: opts.FirstRecord, Last: opts.GetMax()},
	)

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawTrace], errorChan chan error) {
		for _, mon := range monitorArray {
			if items, err := opts.readTraces(monitorArray, &mon, filter, errorChan, abiCache); err != nil {
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

func (opts *ExportOptions) readTraces(
	monitorArray []monitor.Monitor,
	mon *monitor.Monitor,
	filter *filter.AppearanceFilter,
	errorChan chan error,
	abiCache *articulate.AbiCache,
) ([]*types.SimpleTrace, error) {
	if txMap, cnt, err := monitor.ReadAppearancesToMap[types.SimpleTransaction](mon, filter); err != nil {
		errorChan <- err
		return nil, err
	} else if !opts.NoZero || cnt > 0 {
		bar := logger.NewBar(logger.BarOptions{
			Prefix:  mon.Address.Hex(),
			Enabled: !opts.Globals.TestMode && len(opts.Globals.File) == 0,
			Total:   mon.Count(),
		})
		if err := opts.Conn.ReadTransactions(txMap, opts.Fourbytes, bar, true /* readTraces */); err != nil { // calls IterateOverMap
			return nil, err
		}

		// Sort the items back into an ordered array by block number
		items := make([]*types.SimpleTrace, 0, len(txMap))
		for _, tx := range txMap {
			for index, trace := range tx.Traces {
				trace := trace
				trace.TraceIndex = uint64(index)
				isCreate := trace.Action.CallType == "creation" || trace.TraceType == "create"
				if !opts.Factory || isCreate {
					if opts.Articulate {
						if err := abiCache.ArticulateTrace(&trace); err != nil {
							errorChan <- fmt.Errorf("error articulating trace: %v", err)
						}
					}
					items = append(items, &trace)
				}
			}
		}
		sort.Slice(items, func(i, j int) bool {
			if opts.Reversed {
				i, j = j, i
			}
			itemI := items[i]
			itemJ := items[j]
			if itemI.BlockNumber == itemJ.BlockNumber {
				if itemI.TransactionIndex == itemJ.TransactionIndex {
					return itemI.TraceIndex < itemJ.TraceIndex
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

/*
TODO: NOTE
        bool isSelfDestruct = trace.action.selfDestructed != "";
        if (isSelfDestruct) {
            copy.action.from = trace.action.selfDestructed;
            copy.action.to = trace.action.refundAddress;
            copy.action.callType = "suicide";
            copy.action.value = trace.action.balance;
            copy.traceAddress.push_back("s");
            copy.transactionHash = uint_2_Hex(trace.blockNumber * 100000 + trace.transactionIndex);
            copy.action.input = "0x";
        }
        cout << ((isJson() && !opt->firstOut) ? ", " : "");
        cout << copy;
        opt->firstOut = false;
        bool isCreation = trace.result.address != "";
        if (isCreation) {
            copy.action.from = "0x0";
            copy.action.to = trace.result.address;
            copy.action.callType = "creation";
            copy.action.value = trace.action.value;
            if (copy.traceAddress.size() == 0)
                copy.traceAddress.push_back("null");
            copy.traceAddress.push_back("s");
            copy.transactionHash = uint_2_Hex(trace.blockNumber * 100000 + trace.transactionIndex);
            copy.action.input = trace.action.input;
            cout << ((isJson() && !opt->firstOut) ? ", " : "");
            cout << copy;
            opt->firstOut = false;

*/
