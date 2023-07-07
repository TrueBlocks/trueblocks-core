// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"context"
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ExportOptions) HandleTraces(monitorArray []monitor.Monitor) error {
	abiCache := articulate.NewAbiCache()
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	sortBy := monitor.Sorted
	if opts.Reversed {
		sortBy = monitor.Reversed
	}
	exportRange := base.FileRange{First: opts.FirstBlock, Last: opts.LastBlock}
	nExported := uint64(0)
	nSeen := int64(-1)

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawTrace], errorChan chan error) {
		visitAppearance := func(app *types.SimpleAppearance) error {
			raw := types.RawAppearance{
				Address:          app.Address.Hex(),
				BlockNumber:      uint32(app.BlockNumber),
				TransactionIndex: uint32(app.TransactionIndex),
			}
			if tx, err := rpcClient.GetTransactionByAppearance(chain, &raw, true); err != nil {
				errorChan <- err
				return nil
			} else {
				matchesFourByte := len(opts.Fourbytes) == 0 // either there is no four bytes...
				for _, fb := range opts.Fourbytes {
					if strings.HasPrefix(tx.Input, fb) {
						matchesFourByte = true // ... or the four bytes match
					}
				}
				if matchesFourByte {
					for _, trace := range tx.Traces {
						trace := trace
						isCreate := trace.Action.CallType == "creation" || trace.TraceType == "create"
						if !opts.Factory || isCreate {
							if opts.Articulate {
								if err = abiCache.ArticulateTrace(chain, &trace); err != nil {
									errorChan <- err // continue even on error
								}
							}
							modelChan <- &trace
						}
					}
				}
				return nil
			}
		}

		for _, mon := range monitorArray {
			if apps, cnt, err := mon.ReadAppearances2(sortBy); err != nil {
				errorChan <- err
				return
			} else if cnt == 0 {
				errorChan <- fmt.Errorf("no appearances found for %s", mon.Address.Hex())
				return
			} else {
				currentBn := uint32(0)
				currentTs := int64(0)
				for i, app := range apps {
					nSeen++
					appRange := base.FileRange{First: uint64(app.BlockNumber), Last: uint64(app.BlockNumber)}
					if appRange.Intersects(exportRange) {
						if nSeen < int64(opts.FirstRecord) {
							logger.Progress(!testMode && true, "Skipping:", nExported, opts.FirstRecord)
							continue
						} else if opts.IsMax(nExported) {
							logger.Progress(!testMode && true, "Quitting:", nExported, opts.FirstRecord)
							return
						}
						nExported++

						logger.Progress(!testMode && nSeen%723 == 0, "Processing: ", mon.Address.Hex(), " ", app.BlockNumber, ".", app.TransactionId)
						if app.BlockNumber != currentBn || app.BlockNumber == 0 {
							currentTs, _ = tslib.FromBnToTs(chain, uint64(app.BlockNumber))
						}
						currentBn = app.BlockNumber

						if err := visitAppearance(&types.SimpleAppearance{
							Address:          mon.Address,
							BlockNumber:      app.BlockNumber,
							TransactionIndex: app.TransactionId,
							Timestamp:        currentTs,
						}); err != nil {
							errorChan <- err
							return
						}

					} else {
						logger.Progress(!testMode && i%100 == 0, "Skipping:", app)
					}
				}
			}
		}
	}

	extra := map[string]interface{}{
		"articulate": opts.Articulate,
		"testMode":   testMode,
		"export":     true,
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
