// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ExportOptions) HandleAccounting(rCtx *output.RenderCtx, monitorArray []monitor.Monitor) error {
	// TODO: BOGUS - RECONSIDER THIS
	opts.Articulate = true

	abiCache := articulate.NewAbiCache(opts.Conn, opts.Articulate)
	filter := types.NewFilter(
		opts.Reversed,
		opts.Reverted,
		opts.Fourbytes,
		base.BlockRange{First: opts.FirstBlock, Last: opts.LastBlock},
		base.RecordRange{First: opts.FirstRecord, Last: opts.GetMax()},
	)
	assetFilters := make([]base.Address, 0, len(opts.Asset))
	for _, asset := range opts.Asset {
		assetFilters = append(assetFilters, base.HexToAddress(asset))
	}

	var recon *ledger.Reconciler
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		visitAppearance := func(node *types.AppNode[types.Transaction], app *types.Appearance) error {
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

					node.SetData(tx)
					if statements, err := recon.GetStatements(node); err != nil {
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
				ledgerOpts := &ledger.ReconcilerOptions{
					AccountFor:   mon.Address,
					FirstBlock:   opts.FirstBlock,
					LastBlock:    opts.LastBlock,
					AsEther:      opts.Globals.Ether,
					UseTraces:    opts.Traces,
					Reversed:     opts.Reversed,
					AssetFilters: assetFilters,
					AppFilters:   filter,
				}

				recon = ledger.NewReconciler(opts.Conn, ledgerOpts)
				list, err := types.NewAppList[types.Transaction](apps, nil)
				if err != nil {
					errorChan <- err
				}
				i := -1
				for node := list.Head; node != nil; node = node.Next() {
					i++
					if err := visitAppearance(node, &apps[i]); err != nil {
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
