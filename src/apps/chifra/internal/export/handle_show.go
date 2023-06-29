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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ExportOptions) HandleShow(monitorArray []monitor.Monitor) error {
	abiCache := articulate.NewAbiCache()
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	exportRange := base.FileRange{First: opts.FirstBlock, Last: opts.LastBlock}
	nExported := uint64(0)
	nSeen := int64(-1)

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawTransaction], errorChan chan error) {
		visitAppearance := func(app *types.SimpleAppearance) error {
			raw := types.RawAppearance{
				Address:          app.Address.Hex(),
				BlockNumber:      uint32(app.BlockNumber),
				TransactionIndex: uint32(app.TransactionIndex),
			}
			if tx, err := rpcClient.GetTransactionByAppearance(chain, &raw, false); err != nil {
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
					modelChan <- tx
				}
				return nil
			}
		}

		for _, mon := range monitorArray {
			count := mon.Count()
			opts.Apps = make([]index.AppearanceRecord, count)
			if err := mon.ReadAppearances(&opts.Apps); err != nil {
				errorChan <- err
				return
			} else if len(opts.Apps) == 0 {
				errorChan <- fmt.Errorf("no appearances found for %s", mon.Address.Hex())
				return
			}
			opts.Sort()

			currentBn := uint32(0)
			currentTs := int64(0)
			for i, app := range opts.Apps {
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
