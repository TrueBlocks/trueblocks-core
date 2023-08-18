// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package filter

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type AppearanceFilter struct {
	OuterBounds base.BlockRange
	sortBy      AppearanceSort
	Reversed    bool
	exportRange base.BlockRange
	recordRange base.RecordRange
	EnableRr    bool
	nSeen       int64
	nExported   uint64
	currentBn   uint32
	currentTs   int64
}

func NewFilter(reversed bool, exportRange base.BlockRange, recordRange base.RecordRange) *AppearanceFilter {
	sortBy := Sorted
	if reversed {
		sortBy = Reversed
	}
	return &AppearanceFilter{
		exportRange: exportRange,
		recordRange: recordRange,
		OuterBounds: base.BlockRange{First: 0, Last: utils.NOPOS},
		sortBy:      sortBy,
		Reversed:    reversed,
		nSeen:       -1,
		EnableRr:    true,
	}
}

func NewEmptyFilter() *AppearanceFilter {
	return NewFilter(
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
	return f.OuterBounds
}

// BlockRangeFilter checks to see if the appearance intersects with the user-supplied --first_block/--last_block pair (if any)
func (f *AppearanceFilter) BlockRangeFilter(app *index.AppearanceRecord) (passed, finished bool) {
	appRange := base.FileRange{First: uint64(app.BlockNumber), Last: uint64(app.BlockNumber)} // --first_block/--last_block
	if !appRange.Intersects(base.FileRange(f.exportRange)) {
		return false, false
	}

	if f.EnableRr {
		return f.RecordCountFilter()
	}

	return true, false
}

// RecordCountFilter checks to see if the appearance is at or later than the --first_record and less than (because it's zero-based) --max_records.
func (f *AppearanceFilter) RecordCountFilter() (passed, finished bool) {
	f.nSeen++

	if f.nSeen < int64(f.recordRange.First) { // --first_record
		logger.Progress(true, "Skipping:", f.nExported, f.recordRange.First)
		return false, false
	}

	if f.nExported >= f.recordRange.Last { // --max_records
		logger.Progress(true, "Quitting:", f.nExported, f.recordRange.First)
		return false, true
	}

	f.nExported++
	return true, false
}
