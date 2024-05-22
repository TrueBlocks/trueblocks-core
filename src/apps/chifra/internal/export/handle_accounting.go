// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"context"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ExportOptions) HandleAccounting(monitorArray []monitor.Monitor) error {
	// TODO: BOGUS - RECONSIDER THIS
	opts.Articulate = true

	ledgers := &ledger.Ledger{}
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
		visitAppearance := func(app *types.Appearance) error {
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

					if statements, err := ledgers.GetStatements(opts.Conn, filter, tx); err != nil {
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
				_ = ledgers.SetContexts(chain, apps)

				for _, app := range apps {
					if err := visitAppearance(&app); err != nil {
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

	extraOpts := map[string]interface{}{
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
		extraOpts["namesMap"] = namesMap
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}
