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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ranges"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ExportOptions) HandleTraces(rCtx *output.RenderCtx, monitorArray []monitor.Monitor) error {
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		abiCache := articulate.NewAbiCache(opts.Conn, opts.Articulate)
		filter := types.NewFilter(
			opts.Reversed,
			opts.Reverted,
			opts.Fourbytes,
			ranges.BlockRange{First: opts.FirstBlock, Last: opts.LastBlock},
			ranges.RecordRange{First: opts.FirstRecord, Last: opts.GetMax()},
		)

		for _, mon := range monitorArray {
			if sliceOfMaps, cnt, err := monitor.AsSliceOfItemMaps[[]*types.Trace](&mon, filter, filter.Reversed); err != nil {
				errorChan <- err
				rCtx.Cancel()

			} else if cnt == 0 {
				errorChan <- fmt.Errorf("no blocks found for the query")
				continue

			} else {
				showProgress := opts.Globals.ShowProgress()
				bar := logger.NewBar(logger.BarOptions{
					Prefix:  mon.Address.Hex(),
					Enabled: showProgress,
					Total:   int64(cnt),
				})

				finished := false
				for _, thisMap := range sliceOfMaps {
					if rCtx.WasCanceled() {
						return
					}

					if finished {
						continue
					}

					for app := range thisMap {
						thisMap[app] = &[]*types.Trace{}
					}

					iterFunc := func(app types.Appearance, value *[]*types.Trace) error {
						if tx, err := opts.Conn.GetTransactionByAppearance(&app, true); err != nil {
							return err
						} else {
							passes := filter.PassesTxFilter(tx)
							if passes {
								for index, trace := range tx.Traces {
									trace.TraceIndex = base.Tracenum(index)
									isCreate := trace.Action.CallType == "creation" || trace.TraceType == "create"
									if !opts.Factory || isCreate {
										if opts.Articulate {
											if err := abiCache.ArticulateTrace(&trace); err != nil {
												errorChan <- fmt.Errorf("error articulating trace: %v", err)
											}
										}
										*value = append(*value, &trace)
									}
								}
							}
							if bar != nil {
								bar.Tick()
							}
							return nil
						}
					}

					iterCtx, iterCancel := context.WithCancel(context.Background())
					defer iterCancel()
					errChan := make(chan error)
					go utils.IterateOverMap(iterCtx, errChan, thisMap, iterFunc)
					if stepErr := <-errChan; stepErr != nil {
						errorChan <- stepErr
						return
					}

					items := make([]*types.Trace, 0, len(thisMap)*2)
					for _, traceSlice := range thisMap {
						if traceSlice != nil && *traceSlice != nil {
							items = append(items, *traceSlice...)
						}
					}
					sort.Slice(items, func(i, j int) bool {
						if opts.Reversed {
							i, j = j, i
						}
						if items[i].BlockNumber == items[j].BlockNumber {
							if items[i].TransactionIndex == items[j].TransactionIndex {
								return items[i].TraceIndex < items[j].TraceIndex
							}
							return items[i].TransactionIndex < items[j].TransactionIndex
						}
						return items[i].BlockNumber < items[j].BlockNumber
					})

					for _, item := range items {
						if item.BlockHash.IsZero() {
							continue
						}
						var passes bool
						passes, finished = filter.PassesCountFilter()
						if passes {
							modelChan <- item
						}
						if finished {
							break
						}
					}
				}
				bar.Finish(true /* newLine */)
			}
		}
	}

	extraOpts := map[string]any{
		"articulate": opts.Articulate,
		"export":     true,
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
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
