// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package listPkg

import (
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/bykof/gostradamus"
)

func (opts *ListOptions) HandleListAppearances(monitorArray []monitor.Monitor) error {
	for _, mon := range monitorArray {
		count := mon.Count()
		apps := make([]index.AppearanceRecord, count)
		err := mon.ReadAppearances(&apps)
		if err != nil {
			return err
		}
		if len(apps) == 0 {
			logger.Warn("No appearances found for", mon.GetAddrStr())
			return nil
		}

		sort.Slice(apps, func(i, j int) bool {
			si := uint64(apps[i].BlockNumber)
			si = (si << 32) + uint64(apps[i].TransactionId)
			sj := uint64(apps[j].BlockNumber)
			sj = (sj << 32) + uint64(apps[j].TransactionId)
			return si < sj
		})

		exportRange := base.FileRange{First: opts.FirstBlock, Last: opts.LastBlock}
		results := make([]types.RawAppearance, 0, mon.Count())
		verboseResults := make([]types.SimpleAppearance, 0, mon.Count())
		for record, app := range apps {
			appRange := base.FileRange{First: uint64(app.BlockNumber), Last: uint64(app.BlockNumber)}
			if appRange.Intersects(exportRange) {
				if opts.Globals.Verbose {
					ts, err := tslib.FromBnToTs(opts.Globals.Chain, uint64(app.BlockNumber))
					if err != nil {
						return err
					}
					s := types.SimpleAppearance{
						Address:          mon.Address,
						BlockNumber:      app.BlockNumber,
						TransactionIndex: app.TransactionId,
						Timestamp:        ts,
						Date:             gostradamus.FromUnixTimestamp(ts).String(),
					}
					if uint64(record+1) >= opts.FirstRecord && (opts.MaxRecords == 250 || uint64(len(verboseResults)) < opts.MaxRecords) {
						verboseResults = append(verboseResults, s)
					}
				} else {
					s := types.RawAppearance{
						Address:          mon.GetAddrStr(),
						BlockNumber:      app.BlockNumber,
						TransactionIndex: app.TransactionId,
					}
					if uint64(record+1) >= opts.FirstRecord && (opts.MaxRecords == 250 || uint64(len(results)) < opts.MaxRecords) {
						results = append(results, s)
					}
				}
			}
		}

		// TODO: Fix export without arrays
		if opts.Globals.Verbose {
			err = globals.RenderSlice(&opts.Globals, verboseResults)
			if err != nil {
				return err
			}
		} else {
			err = globals.RenderSlice(&opts.Globals, results)
			if err != nil {
				return err
			}
		}
	}
	return nil
}
