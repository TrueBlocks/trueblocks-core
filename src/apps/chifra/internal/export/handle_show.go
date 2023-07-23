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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ExportOptions) HandleShow(monitorArray []monitor.Monitor) error {
	if opts.Accounting {
		// TODO: BOGUS - RECONSIDER THIS
		opts.Articulate = true
	}

	ledgers := &ledger.Ledger{}
	abiCache := articulate.NewAbiCache()
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
	rpcOptions := opts.Globals.DefaultRpcOptions(nil)

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawTransaction], errorChan chan error) {
		visitAppearance := func(app *types.SimpleAppearance) error {
			raw := types.RawAppearance{
				Address:          app.Address.Hex(),
				BlockNumber:      uint32(app.BlockNumber),
				TransactionIndex: uint32(app.TransactionIndex),
			}
			if tx, err := rpcClient.GetTransactionByAppearance(chain, &raw, false, rpcOptions); err != nil {
				errorChan <- err
				return nil
			} else {
				matches := len(opts.Fourbytes) == 0 // either there is no four bytes...
				for _, fb := range opts.Fourbytes {
					if strings.HasPrefix(tx.Input, fb) {
						matches = true
					}
				}
				if matches {
					if opts.Articulate {
						if err = abiCache.ArticulateTx(chain, tx); err != nil {
							errorChan <- err // continue even on error
						}
					}

					if opts.Accounting {
						if statements, err := ledgers.GetStatementsFromAppearance(chain, &raw, rpcOptions); err != nil {
							errorChan <- err
						} else {
							tx.Statements = &statements
						}
					}

					modelChan <- tx
				}
				return nil
			}
		}

		for _, mon := range monitorArray {
			if apps, cnt, err := mon.ReadAndFilterAppearances(filter); err != nil {
				errorChan <- err
				return
			} else if !opts.NoZero || cnt > 0 {
				ledgers = ledger.NewLedger(
					chain,
					mon.Address,
					opts.FirstBlock,
					opts.LastBlock,
					opts.Globals.Ether,
					testMode,
					opts.NoZero,
					opts.Traces,
					&opts.Asset,
				)
				if opts.Accounting {
					ledgers.SetContexts(chain, apps, filter.GetOuterBounds())
				}

				for _, app := range apps {
					app := app
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

	extra := map[string]interface{}{
		"articulate": opts.Articulate,
		"testMode":   testMode,
		"export":     true,
		"ether":      opts.Globals.Ether,
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
