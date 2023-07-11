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

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawReceipt], errorChan chan error) {
		for _, mon := range monitorArray {
			if theMap, cnt, err := monitor.ReadAppearancesToMap[types.SimpleTransaction](&mon, filter); err != nil {
				errorChan <- err
				return
			} else if !opts.NoZero || cnt > 0 {
				if items, err := opts.readReceipts(&mon, theMap, abiCache); err != nil {
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

func (opts *ExportOptions) readReceipts(mon *monitor.Monitor, theMap map[types.SimpleAppearance]*types.SimpleTransaction, abiCache *articulate.AbiCache) ([]*types.SimpleReceipt, error) {
	if err := opts.readTransactions(mon, theMap, false); err != nil {
		return nil, err
	}

	// Sort the items back into an ordered array by block number
	items := make([]*types.SimpleReceipt, 0, len(theMap))
	for _, v := range theMap {
		if v.Receipt == nil {
			continue
		}
		items = append(items, v.Receipt)
	}
	sort.Slice(items, func(i, j int) bool {
		if items[i].BlockNumber == items[j].BlockNumber {
			return items[i].TransactionIndex < items[j].TransactionIndex
		}
		return items[i].BlockNumber < items[j].BlockNumber
	})

	// Return the array of items
	return items, nil
}
