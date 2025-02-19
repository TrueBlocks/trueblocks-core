// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ExportOptions) HandleAccounting(rCtx *output.RenderCtx, monitorArray []monitor.Monitor) error {
	// TODO: BOGUS - RECONSIDER THIS
	opts.Articulate = true

	ledgers := &ledger.Ledger{}
	abiCache := articulate.NewAbiCache(opts.Conn, opts.Articulate)
	testMode := opts.Globals.TestMode
	filter := filter.NewFilter(
		opts.Reversed,
		opts.Reverted,
		opts.Fourbytes,
		base.BlockRange{First: opts.FirstBlock, Last: opts.LastBlock},
		base.RecordRange{First: opts.FirstRecord, Last: opts.GetMax()},
	)

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		visitAppearance := func(pos *types.AppPosition, app *types.Appearance) error {
			if tx, err := opts.Conn.GetTransactionByAppearance(app, false); err != nil {
				errorChan <- err
				return nil

			} else {
				passes, _ := filter.ApplyTxFilters(tx)
				if passes {
					if opts.Articulate {
						if err = abiCache.ArticulateTransaction(tx); err != nil {
							errorChan <- err // continue even on error
						}
					}

					if statements, err := ledgers.GetStatements(pos, filter, tx); err != nil {
						errorChan <- err

					} else {
						tx.Statements = &statements
					}

					modelChan <- tx
				}
				return nil
			}
		}

		for _, mon := range monitorArray {
			if apps, cnt, err := mon.ReadAndFilterAppearances(filter, true /* withCount */); err != nil {
				errorChan <- err
				return

			} else if !opts.NoZero || cnt > 0 {
				ledgers = ledger.NewLedger(
					opts.Conn,
					mon.Address,
					opts.FirstBlock,
					opts.LastBlock,
					opts.Globals.Ether,
					testMode,
					opts.NoZero,
					opts.Traces,
					opts.Reversed,
					&opts.Asset,
				)

				for i, app := range apps {
					prev := uint32(0)
					if apps[i].BlockNumber > 0 {
						prev = apps[i].BlockNumber - 1
					}
					if i > 0 {
						prev = apps[i-1].BlockNumber
					}
					next := apps[i].BlockNumber + 1
					if i < len(apps)-1 {
						next = apps[i+1].BlockNumber
					}
					pos := &types.AppPosition{
						Prev:  base.Blknum(prev),
						Next:  base.Blknum(next),
						First: i == 0,
						Last:  i == len(apps)-1,
					}
					if err := visitAppearance(pos, &app); err != nil {
						errorChan <- err
						return
					}
				}

			} else {
				errorChan <- fmt.Errorf("no appearances found for %s", mon.Address.Hex())
				continue
			}
		}
	}

	extraOpts := map[string]any{
		"articulate": opts.Articulate,
		"export":     true,
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}
