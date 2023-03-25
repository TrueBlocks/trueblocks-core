// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package listPkg

import (
	"context"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ListOptions) HandleListAppearances(monitorArray []monitor.Monitor) error {
	chain := opts.Globals.Chain
	exportRange := base.FileRange{First: opts.FirstBlock, Last: opts.LastBlock}
	nExported := uint64(1)
	nSeen := uint64(0)

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawAppearance], errorChan chan error) {
		for _, mon := range monitorArray {
			count := mon.Count()
			apps := make([]index.AppearanceRecord, count)
			err := mon.ReadAppearances(&apps)
			if err != nil {
				errorChan <- err
				return
			}
			if len(apps) == 0 {
				logger.Warn("No appearances found for", mon.GetAddrStr())
				continue
			}

			sort.Slice(apps, func(i, j int) bool {
				si := uint64(apps[i].BlockNumber)
				si = (si << 32) + uint64(apps[i].TransactionId)
				sj := uint64(apps[j].BlockNumber)
				sj = (sj << 32) + uint64(apps[j].TransactionId)
				return si < sj
			})

			currentBn := uint32(0)
			currentTs := int64(0)
			for i, app := range apps {
				nSeen++
				appRange := base.FileRange{First: uint64(app.BlockNumber), Last: uint64(app.BlockNumber)}
				if appRange.Intersects(exportRange) {
					if nSeen < opts.FirstRecord {
						// logger.Info("Skipping:", nExported, opts.FirstRecord)
						continue
					} else if nExported > opts.MaxRecords {
						// logger.Info("Quitting:", nExported, opts.FirstRecord)
						return
					}
					nExported++

					s := types.SimpleAppearance{
						Address:          mon.Address,
						BlockNumber:      app.BlockNumber,
						TransactionIndex: app.TransactionId,
					}

					if s.BlockNumber != currentBn {
						currentTs, _ = tslib.FromBnToTs(chain, uint64(s.BlockNumber))
					}
					currentBn = s.BlockNumber
					s.Timestamp = currentTs
					s.Date = utils.FormattedDate(currentTs)

					modelChan <- &s

				} else if i%100 == 0 && !opts.Globals.TestMode {
					logger.Progress("Skipping:", app)
				}
			}
		}
	}

	return output.StreamMany(ctx, fetchData, output.OutputOptions{
		Writer:     opts.Globals.Writer,
		Chain:      opts.Globals.Chain,
		TestMode:   opts.Globals.TestMode,
		NoHeader:   opts.Globals.NoHeader,
		ShowRaw:    opts.Globals.ShowRaw,
		Verbose:    opts.Globals.Verbose,
		LogLevel:   opts.Globals.LogLevel,
		Format:     opts.Globals.Format,
		OutputFn:   opts.Globals.OutputFn,
		Append:     opts.Globals.Append,
		JsonIndent: "  ",
	})
}
