// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package filter

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type AppearanceFilter struct {
	Chain       string
	ExportRange base.BlockRange
	RecordRange base.RecordRange
	OuterBounds base.BlockRange
	SortBy      AppearanceSort
	Reversed    bool
	Logging     bool
	ReadTs      bool
	nSeen       int64
	nExported   uint64
	currentBn   uint32
	currentTs   int64
}

func NewFilter(chain string, readTs, reversed, logging bool, exportRange base.BlockRange, recordRange base.RecordRange) *AppearanceFilter {
	sortBy := Sorted
	if reversed {
		sortBy = Reversed
	}
	return &AppearanceFilter{
		Chain:       chain,
		ExportRange: exportRange,
		RecordRange: recordRange,
		OuterBounds: base.BlockRange{First: 0, Last: utils.NOPOS},
		SortBy:      sortBy,
		Reversed:    reversed,
		Logging:     logging,
		ReadTs:      readTs,
		nSeen:       -1,
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
	f.SortBy = sortBy
}

func (f *AppearanceFilter) Reset() {
	f.currentBn = uint32(0)
	f.currentTs = int64(0)
}

func (f *AppearanceFilter) GetOuterBounds() base.BlockRange {
	return f.OuterBounds
}

func (f *AppearanceFilter) GetTimestamp(chain string, bn uint32) int64 {
	if bn != f.currentBn || bn == 0 {
		f.currentTs, _ = tslib.FromBnToTs(chain, uint64(bn))
	}
	f.currentBn = bn
	return f.currentTs
}

func (f *AppearanceFilter) Passes(address base.Address, app *index.AppearanceRecord, previous *index.AppearanceRecord) (passed, finished bool) {
	f.nSeen++
	appRange := base.FileRange{First: uint64(app.BlockNumber), Last: uint64(app.BlockNumber)}
	if !appRange.Intersects(base.FileRange(f.ExportRange)) {
		return false, false
	}

	if f.nSeen < int64(f.RecordRange.First) {
		logger.Progress(true, "Skipping:", f.nExported, f.RecordRange.First)
		return false, false
	}

	if f.nExported >= f.RecordRange.Last {
		logger.Progress(true, "Quitting:", f.nExported, f.RecordRange.First)
		return false, true
	}

	f.nExported++
	return true, false
}
