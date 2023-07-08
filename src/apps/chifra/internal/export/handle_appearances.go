// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"context"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ExportOptions) HandleAppearances(monitorArray []monitor.Monitor) error {
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
	fetchData := func(modelChan chan types.Modeler[types.RawAppearance], errorChan chan error) {
		visitAppearance := func(app *types.SimpleAppearance) error {
			modelChan <- app
			return nil
		}

		for _, mon := range monitorArray {
			if apps, cnt, err := mon.ReadAppearancesToSlice(sortBy); err != nil {
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
						s := types.SimpleAppearance{
							Address:          mon.Address,
							BlockNumber:      app.BlockNumber,
							TransactionIndex: app.TransactionId,
							Timestamp:        utils.NOPOSI,
						}
						if opts.Globals.Verbose {
							if app.BlockNumber != currentBn || app.BlockNumber == 0 {
								currentTs, _ = tslib.FromBnToTs(chain, uint64(app.BlockNumber))
							}
							currentBn = app.BlockNumber
							s.Timestamp = currentTs
						}

						if err := visitAppearance(&s); err != nil {
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

func (opts *ExportOptions) IsMax(cnt uint64) bool {
	max := opts.MaxRecords
	if max == 250 && !opts.Globals.IsApiMode() {
		max = utils.NOPOS
	}
	return cnt >= max
}
