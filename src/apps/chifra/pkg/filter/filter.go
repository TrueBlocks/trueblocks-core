// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package filter

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type AppearanceFilter struct {
	OuterBounds base.BlockRange
	sortBy      AppearanceSort
	Reversed    bool
	reverted    bool
	fourBytes   []string
	exportRange base.BlockRange
	recordRange base.RecordRange
	nSeen       int64
	nExported   uint64
	currentBn   uint32
	currentTs   int64
	BlocksOnly  bool
}

func NewFilter(reversed, reverted bool, fourBytes []string, exportRange base.BlockRange, recordRange base.RecordRange) *AppearanceFilter {
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
		reverted:    reverted,
		fourBytes:   fourBytes,
		nSeen:       -1,
	}
}

func NewEmptyFilter() *AppearanceFilter {
	return NewFilter(
		false,
		false,
		[]string{},
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

// ApplyFilter checks to see if the appearance intersects with the user-supplied --first_block/--last_block pair (if any)
func (f *AppearanceFilter) ApplyFilter(app *types.AppRecord) (passed, finished bool) {
	appRange := base.FileRange{First: uint64(app.BlockNumber), Last: uint64(app.BlockNumber)} // --first_block/--last_block
	if !appRange.Intersects(base.FileRange(f.exportRange)) {
		return false, false
	}
	return f.ApplyCountFilter()
}

// ApplyRangeFilter checks to see if the appearance intersects with the user-supplied --first_block/--last_block pair (if any)
func (f *AppearanceFilter) ApplyRangeFilter(app *types.AppRecord) (passed, finished bool) {
	appRange := base.FileRange{First: uint64(app.BlockNumber), Last: uint64(app.BlockNumber)} // --first_block/--last_block
	return appRange.Intersects(base.FileRange(f.exportRange)), false
}

// ApplyCountFilter checks to see if the appearance is at or later than the --first_record and less than (because it's zero-based) --max_records.
func (f *AppearanceFilter) ApplyCountFilter() (passed, finished bool) {
	f.nSeen++

	if f.nSeen < int64(f.recordRange.First) { // --first_record
		return false, false
	}

	if f.nExported >= f.recordRange.Last { // --max_records
		return false, true
	}

	f.nExported++
	return true, false
}

// ApplyTxFilters applies other filters such as the four byte and reverted filters.
func (f *AppearanceFilter) ApplyTxFilters(tx *types.Transaction) (passed, finished bool) {
	matchesReverted := !f.reverted || tx.IsError
	matchesFourbyte := len(f.fourBytes) == 0
	for _, fourBytes := range f.fourBytes {
		if strings.HasPrefix(tx.Input, fourBytes) {
			matchesFourbyte = true
			break
		}
	}
	// fmt.Println("len:", len(f.fourBytes), "matchesFourbyte", matchesFourbyte, "matchesReverted", matchesReverted)
	return matchesFourbyte && matchesReverted, false
}

func (f *AppearanceFilter) ApplyLogFilter(log *types.Log, addrArray []base.Address) bool {
	haystack := make([]byte, 66*len(log.Topics)+len(log.Data))
	haystack = append(haystack, log.Address.Hex()[2:]...)
	for _, topic := range log.Topics {
		haystack = append(haystack, topic.Hex()[2:]...)
	}
	haystack = append(haystack, log.Data[2:]...)

	for _, addr := range addrArray {
		if strings.Contains(string(haystack), addr.Hex()[2:]) {
			return true
		}
	}
	return false
}
