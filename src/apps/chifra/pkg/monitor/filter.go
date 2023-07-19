// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitor

import (
	"encoding/binary"
	"fmt"
	"io"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type AppearanceFilter struct {
	chain       string
	reversed    bool
	exportRange base.BlockRange
	recordRange base.RecordRange
	outerBounds base.BlockRange
	logging     bool
	readTs      bool
	nSeen       int64
	nExported   uint64
	currentBn   uint32
	currentTs   int64
	sortBy      AppearanceSort
}

func NewFilter(chain string, readTs, reversed, logging bool, exportRange base.BlockRange, recordRange base.RecordRange) *AppearanceFilter {
	sortBy := Sorted
	if reversed {
		sortBy = Reversed
	}
	return &AppearanceFilter{
		chain:       chain,
		reversed:    reversed,
		exportRange: exportRange,
		recordRange: recordRange,
		outerBounds: base.BlockRange{First: 0, Last: utils.NOPOS},
		logging:     logging,
		readTs:      readTs,
		nSeen:       -1,
		sortBy:      sortBy,
	}
}

func NewEmptyFilter(chain string) *AppearanceFilter {
	return NewFilter(
		chain,
		false,
		false,
		false,
		base.BlockRange{First: 0, Last: utils.NOPOS},
		base.RecordRange{First: 0, Last: utils.NOPOS},
	)
}

func (f *AppearanceFilter) SetSort(sortBy AppearanceSort) {
	f.sortBy = sortBy
}

func (f *AppearanceFilter) Reset() {
	f.currentBn = uint32(0)
	f.currentTs = int64(0)
}

func (f *AppearanceFilter) GetOuterBounds() base.BlockRange {
	return f.outerBounds
}

func (f *AppearanceFilter) GetTimestamp(chain string, bn uint32) int64 {
	if bn != f.currentBn || bn == 0 {
		f.currentTs, _ = tslib.FromBnToTs(chain, uint64(bn))
	}
	f.currentBn = bn
	return f.currentTs
}

func (mon *Monitor) ReadAndFilterAppearances(filter *AppearanceFilter) (apps []types.SimpleAppearance, cnt int, err error) {
	readAppearances := func(apps *[]index.AppearanceRecord) (err error) {
		if int64(len(*apps)) > mon.Count() {
			err = fmt.Errorf("array is larger than the size of the file (%d,%d)", len(*apps), mon.Count())
			return
		}

		if mon.ReadFp == nil {
			path := mon.Path()
			mon.ReadFp, err = os.OpenFile(path, os.O_RDONLY, 0644)
			if err != nil {
				return
			}
		}

		// Seek past the header to get to the first record
		_, err = mon.ReadFp.Seek(index.AppRecordWidth, io.SeekStart)
		if err != nil {
			return
		}

		err = binary.Read(mon.ReadFp, binary.LittleEndian, apps)
		if err != nil {
			return
		}

		return
	}

	filter.Reset()

	if mon.Count() == 0 {
		return nil, 0, nil
	}

	fromDisc := make([]index.AppearanceRecord, mon.Count())
	if err := readAppearances(&fromDisc); err != nil {
		return nil, 0, err
	} else if len(fromDisc) == 0 {
		return nil, 0, nil
	}
	filter.Sort(fromDisc)

	prev := fromDisc[0]
	apps = make([]types.SimpleAppearance, 0, len(fromDisc))
	for i, app := range fromDisc {
		app := app
		if passed, finished := filter.passes(mon.Address, &app); finished {
			return apps, len(apps), nil
		} else if passed {
			if len(apps) == 0 {
				filter.outerBounds.First = uint64(prev.BlockNumber)
			}
			filter.outerBounds.Last = uint64(app.BlockNumber + 1)
			s := types.SimpleAppearance{
				Address:          mon.Address,
				BlockNumber:      uint32(app.BlockNumber),
				TransactionIndex: uint32(app.TransactionId),
				Timestamp:        utils.NOPOSI,
			}
			if filter.readTs {
				s.Timestamp = filter.GetTimestamp(filter.chain, app.BlockNumber)
			}
			apps = append(apps, s)
		} else {
			logger.Progress(filter.logging && i%100 == 0, "Skipping:", app)
		}
		prev = app
	}
	mon.Close()

	return apps, len(apps), nil
}

func (f *AppearanceFilter) passes(address base.Address, app *index.AppearanceRecord) (passed, finished bool) {
	f.nSeen++
	appRange := base.FileRange{First: uint64(app.BlockNumber), Last: uint64(app.BlockNumber)}
	if !appRange.Intersects(base.FileRange(f.exportRange)) {
		return false, false
	}

	if f.nSeen < int64(f.recordRange.First) {
		logger.Progress(true, "Skipping:", f.nExported, f.recordRange.First)
		return false, false
	}

	if f.nExported >= f.recordRange.Last {
		logger.Progress(true, "Quitting:", f.nExported, f.recordRange.First)
		return false, true
	}

	f.nExported++
	return true, false
}
