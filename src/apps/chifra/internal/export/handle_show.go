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

func (opts *ExportOptions) HandleShow(monitorArray []monitor.Monitor) error {
	chain := opts.Globals.Chain
	abiCache := articulate.NewAbiCache(opts.Conn, opts.Articulate)
	testMode := opts.Globals.TestMode
	filter := filter.NewFilter(
		opts.Reversed,
		opts.Reverted,
		opts.Fourbytes,
		base.BlockRange{First: opts.FirstBlock, Last: opts.LastBlock},
		base.RecordRange{First: opts.FirstRecord, Last: opts.GetMax()},
	)

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawTransaction], errorChan chan error) {
		for _, mon := range monitorArray {
			var cnt int
			var err error
			var appMap map[types.SimpleAppearance]*types.SimpleTransaction
			if appMap, cnt, err = monitor.AsMap[types.SimpleTransaction](&mon, filter); err != nil {
				errorChan <- err
				return
			} else if !opts.NoZero || cnt > 0 {
				bar := logger.NewBar(logger.BarOptions{
					Prefix:  mon.Address.Hex(),
					Enabled: !opts.Globals.TestMode,
					Total:   mon.Count(),
				})
				if err := opts.readTransactions(appMap, filter, bar, false /* readTraces */); err != nil { // calls IterateOverMap
					errorChan <- err
					return
				}

				items := make([]*types.SimpleTransaction, 0, len(appMap))
				for _, tx := range appMap {
					if opts.Articulate {
						if err = abiCache.ArticulateTransaction(tx); err != nil {
							errorChan <- err // continue even on error
						}
					}
					items = append(items, tx)
				}
				sort.Slice(items, func(i, j int) bool {
					if opts.Reversed {
						i, j = j, i
					}
					if items[i].BlockNumber == items[j].BlockNumber {
						return items[i].TransactionIndex < items[j].TransactionIndex
					}
					return items[i].BlockNumber < items[j].BlockNumber
				})
				for _, item := range items {
					item := item
					if !item.BlockHash.IsZero() {
						modelChan <- item
					}
				}

			} else {
				errorChan <- fmt.Errorf("no appearances found for %s", mon.Address.Hex())
				return
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
