// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package types

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ranges"
)

type AppearanceFilter struct {
	OuterBounds ranges.BlockRange
	sortBy      AppearanceSort
	Reversed    bool
	reverted    bool
	fourBytes   []string
	exportRange ranges.BlockRange
	recordRange ranges.RecordRange
	nSeen       int64
	nExported   uint64
	currentBn   uint32
	currentTs   int64
	BlocksOnly  bool
}

func NewFilter(reversed, reverted bool, fourBytes []string, exportRange ranges.BlockRange, recordRange ranges.RecordRange) *AppearanceFilter {
	sortBy := Sorted
	if reversed {
		sortBy = Reversed
	}
	return &AppearanceFilter{
		exportRange: exportRange,
		recordRange: recordRange,
		OuterBounds: ranges.BlockRange{First: 0, Last: base.NOPOSN},
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
		ranges.BlockRange{First: 0, Last: base.NOPOSN},
		ranges.RecordRange{First: 0, Last: base.NOPOS},
	)
}

func (f *AppearanceFilter) SetSort(sortBy AppearanceSort) {
	f.sortBy = sortBy
}

func (f *AppearanceFilter) Reset() {
	f.currentBn = uint32(0)
	f.currentTs = int64(0)
}

func (f *AppearanceFilter) GetOuterBounds() ranges.BlockRange {
	return f.OuterBounds
}

// ApplyFilter checks to see if the appearance intersects with the user-supplied --first_block/--last_block pair (if any)
func (f *AppearanceFilter) ApplyFilter(app *AppRecord) (passed, finished bool) {
	appRange := ranges.FileRange{First: base.Blknum(app.BlockNumber), Last: base.Blknum(app.BlockNumber)} // --first_block/--last_block
	if !appRange.Intersects(ranges.FileRange(f.exportRange)) {
		return false, false
	}
	return f.ApplyCountFilter()
}

// ApplyRangeFilter checks to see if the appearance intersects with the user-supplied --first_block/--last_block pair (if any)
func (f *AppearanceFilter) ApplyRangeFilter(app *AppRecord) (passed, finished bool) {
	appRange := ranges.FileRange{First: base.Blknum(app.BlockNumber), Last: base.Blknum(app.BlockNumber)} // --first_block/--last_block
	return appRange.Intersects(ranges.FileRange(f.exportRange)), false
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
func (f *AppearanceFilter) ApplyTxFilters(tx *Transaction) (passed, finished bool) {
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

func (f *AppearanceFilter) ApplyLogFilter(log *Log, addrArray []base.Address) bool {
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
