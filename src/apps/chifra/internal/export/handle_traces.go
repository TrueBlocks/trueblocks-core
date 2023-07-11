// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"context"
	"fmt"
	"os"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ExportOptions) HandleTraces(monitorArray []monitor.Monitor) error {
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

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawTrace], errorChan chan error) {
		for _, mon := range monitorArray {
			if theMap, cnt, err := monitor.ReadAppearancesToMap[types.SimpleTransaction](&mon, filter); err != nil {
				errorChan <- err
				return
			} else if !opts.NoZero || cnt > 0 {
				if items, err := opts.readTraces(&mon, theMap, abiCache); err != nil {
					errorChan <- err
					continue
				} else {
					for _, item := range items {
						item := item
						modelChan <- item
					}
				}
			} else {
				errorChan <- fmt.Errorf("no appearances found for %s", mon.Address.Hex())
				continue
			}
		}
	}

	extra := map[string]interface{}{
		"testMode":   testMode,
		"articulate": opts.Articulate,
		"export":     true,
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

func (opts *ExportOptions) readTraces(mon *monitor.Monitor, theMap map[types.SimpleAppearance]*types.SimpleTransaction, abiCache *articulate.AbiCache) ([]*types.SimpleTrace, error) {
	chain := opts.Globals.Chain
	if err := opts.readTransactions(mon, theMap, true); err != nil {
		return nil, err
	}

	// Sort the items back into an ordered array by block number
	items := make([]*types.SimpleTrace, 0, len(theMap))
	for _, v := range theMap {
		for index, trace := range v.Traces {
			trace := trace
			trace.TraceIndex = uint64(index)
			isCreate := trace.Action.CallType == "creation" || trace.TraceType == "create"
			if !opts.Factory || isCreate {
				if opts.Articulate {
					if err := abiCache.ArticulateTrace(chain, &trace); err != nil {
						// TODO: BOGUS - we need the errorChan
						fmt.Fprintf(os.Stderr, "error articulating trace: %v\n", err)
					}
				}
				items = append(items, &trace)
			}
		}
	}
	sort.Slice(items, func(i, j int) bool {
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
