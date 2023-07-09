// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type AppearanceFilter struct {
	reversed    bool
	currentBn   uint32
	currentTs   int64
	exportRange base.FileRange
	nSeen       int64
	nExported   uint64
	firstRecord uint64
	maxRecords  uint64
	logging     bool
}

func (f *AppearanceFilter) Reset() {
	f.currentBn = uint32(0)
	f.currentTs = int64(0)
}

func (f *AppearanceFilter) GetTimestamp(chain string, bn uint32) int64 {
	if bn != f.currentBn || bn == 0 {
		f.currentTs, _ = tslib.FromBnToTs(chain, uint64(bn))
	}
	f.currentBn = bn
	return f.currentTs
}

func (f *AppearanceFilter) Sort2(apps []types.SimpleAppearance) {
	sortBy := monitor.Sorted
	if f.reversed {
		sortBy = monitor.Reversed
	}
	if sortBy == monitor.Sorted || sortBy == monitor.Reversed {
		sort.Slice(apps, func(i, j int) bool {
			si := (uint64(apps[i].BlockNumber) << 32) + uint64(apps[i].TransactionIndex)
			sj := (uint64(apps[j].BlockNumber) << 32) + uint64(apps[j].TransactionIndex)
			if sortBy == monitor.Reversed {
				return sj < si
			} else {
				return si < sj
			}
		})
	}
}

func ReadAppearancesToSlice2(mon *monitor.Monitor, filter *AppearanceFilter) (apps []types.SimpleAppearance, cnt int, err error) {
	if mon.Count() == 0 {
		return nil, 0, nil
	}

	fromDisc := make([]index.AppearanceRecord, mon.Count())
	if err := mon.ReadAppearances(&fromDisc); err != nil {
		return nil, 0, err
	} else if len(fromDisc) == 0 {
		return nil, 0, nil
	}

	filter.Reset()
	apps = make([]types.SimpleAppearance, 0, len(fromDisc))
	for i, app := range fromDisc {
		app := app
		if passed, finished := filter.passes(mon.Address, &app); finished {
			return apps, len(apps), nil
		} else if passed {
			apps = append(apps, types.SimpleAppearance{
				Address:          mon.Address,
				BlockNumber:      uint32(app.BlockNumber),
				TransactionIndex: uint32(app.TransactionId),
				Timestamp:        utils.NOPOSI,
			})
		} else {
			logger.Progress(filter.logging && i%100 == 0, "Skipping:", app)
		}
	}

	filter.Sort2(apps)
	mon.Close()

	return apps, len(apps), nil
}

func (f *AppearanceFilter) passes(address base.Address, app *index.AppearanceRecord) (passed, finished bool) {
	f.nSeen++
	appRange := base.FileRange{First: uint64(app.BlockNumber), Last: uint64(app.BlockNumber)}
	if !appRange.Intersects(f.exportRange) {
		return false, false
	}

	if f.nSeen < int64(f.firstRecord) {
		logger.Progress(true, "Skipping:", f.nExported, f.firstRecord)
		return false, false
	}

	if f.nExported >= f.maxRecords {
		logger.Progress(true, "Quitting:", f.nExported, f.firstRecord)
		return false, true
	}

	f.nExported++
	logger.Progress(f.logging && f.nSeen%723 == 0, "Processing: ", address.Hex(), " ", app.BlockNumber, ".", app.TransactionId)
	return true, false
}
