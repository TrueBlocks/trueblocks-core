// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"context"
	"fmt"
	"math/big"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ExportOptions) HandleBalances(monitorArray []monitor.Monitor) error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	filter := monitor.NewFilter(
		chain,
		true,
		opts.Reversed,
		!testMode,
		base.BlockRange{First: opts.FirstBlock, Last: opts.LastBlock},
		base.RecordRange{First: opts.FirstRecord, Last: opts.GetMax()},
	)

	// TODO: Why does this have to dirty the caller?
	settings := rpcClient.Settings{
		Chain: chain,
		Opts:  opts,
	}
	opts.Conn = settings.GetRpcConnection()

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

func (opts *ExportOptions) readBalances(
	mon *monitor.Monitor,
	filter *monitor.AppearanceFilter,
	errorChan chan error,
) ([]*types.SimpleToken, error) {
	if txMap, cnt, err := monitor.ReadAppearancesToMap[types.SimpleToken](mon, filter); err != nil {
		errorChan <- err
		return nil, err
	} else if !opts.NoZero || cnt > 0 {
		showProgress := !opts.Globals.TestMode
		var bar = logger.NewBar(mon.Address.Hex(), showProgress, mon.Count())

		// This is called concurrently, once for each appearance
		iterFunc := func(app types.SimpleAppearance, value *types.SimpleToken) error {
			if b, err := opts.Conn.GetBalanceAt(mon.Address, uint64(app.BlockNumber)); err != nil {
				return err
			} else {
				value.Address = base.FAKE_ETH_ADDRESS
				value.Holder = mon.Address
				value.BlockNumber = uint64(app.BlockNumber)
				value.TransactionIndex = uint64(app.TransactionIndex)
				value.Balance = *b
				value.Timestamp = app.Timestamp
				bar.Tick()
				return nil
			}
		}

		// Set up and interate over the map calling iterFunc for each appearance
		ctx, cancel := context.WithCancel(context.Background())
		defer cancel()
		errChan := make(chan error)
		go utils.IterateOverMap(ctx, errChan, txMap, iterFunc)
		if stepErr := <-errChan; stepErr != nil {
			return nil, stepErr
		} else {
			bar.Finish(true)
		}

		// Sort the items back into an ordered array by block number
		items := make([]*types.SimpleToken, 0, len(txMap))
		for _, tx := range txMap {
			items = append(items, tx)
		}
		sort.Slice(items, func(i, j int) bool {
			return items[i].BlockNumber < items[j].BlockNumber
		})

		// Return the array of items
		return items, nil
	} else {
		errorChan <- fmt.Errorf("no appearances found for %s", mon.Address.Hex())
		return nil, nil
	}
}
