// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"context"
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ExportOptions) HandleBalances(monitorArray []monitor.Monitor) error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	filter := filter.NewFilter(
		chain,
		true,
		opts.Reversed,
		!testMode,
		base.BlockRange{First: opts.FirstBlock, Last: opts.LastBlock},
		base.RecordRange{First: opts.FirstRecord, Last: opts.GetMax()},
	)

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawToken], errorChan chan error) {
		for _, mon := range monitorArray {
			if items, err := opts.readBalances(&mon, filter, errorChan); err != nil {
				errorChan <- err
				continue
			} else {
				prevBalance, _ := opts.Conn.GetBalanceAt(mon.Address, filter.GetOuterBounds().First)
				for index, item := range items {
					item := item
					item.PriorBalance = *prevBalance
					if opts.Globals.Verbose || index == 0 || item.PriorBalance.Cmp(&item.Balance) != 0 {
						item.Diff = *big.NewInt(0).Sub(&item.Balance, &item.PriorBalance)
						modelChan <- item
						prevBalance = &item.Balance
					}
				}
			}
		}
	}

	extra := map[string]interface{}{
		"testMode": testMode,
		"export":   true,
		"parts":    []string{"blockNumber", "date", "holder", "balance", "diff", "units"},
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
