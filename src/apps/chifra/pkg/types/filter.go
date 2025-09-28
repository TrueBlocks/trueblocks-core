// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package types

import (
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ranges"
)

type AppearanceSort int

const (
	NotSorted AppearanceSort = iota
	Sorted
	Reversed
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

// PassesFilter checks to see if the appearance intersects with the user-supplied --first_block/--last_block pair (if any)
func (f *AppearanceFilter) PassesFilter(app *AppRecord) (passed, finished bool) {
	appRange := ranges.FileRange{First: base.Blknum(app.BlockNumber), Last: base.Blknum(app.BlockNumber)} // --first_block/--last_block
	if !appRange.Intersects(ranges.FileRange(f.exportRange)) {
		return false, false
	}
	return f.PassesCountFilter()
}

// PassesRangeFilter checks to see if the appearance intersects with the user-supplied --first_block/--last_block pair (if any)
func (f *AppearanceFilter) PassesRangeFilter(app *AppRecord) (passed, finished bool) {
	appRange := ranges.FileRange{First: base.Blknum(app.BlockNumber), Last: base.Blknum(app.BlockNumber)} // --first_block/--last_block
	return appRange.Intersects(ranges.FileRange(f.exportRange)), false
}

// PassesCountFilter checks to see if the appearance is at or later than
// the --first_record and less than (because it's zero-based) --max_records.
func (f *AppearanceFilter) PassesCountFilter() (passed, finished bool) {
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

// PassesTxFilter applies the four byte and reverted filters.
func (f *AppearanceFilter) PassesTxFilter(tx *Transaction) bool {
	// We're looking for reverted txs. This one isn't, so fail.
	if f.reverted && !tx.IsError {
		return false
	}

	// We're not filtering by four byte, so pass it.
	if len(f.fourBytes) == 0 {
		return true
	}

	for _, fourBytes := range f.fourBytes {
		if strings.HasPrefix(tx.Input, fourBytes) {
			return true
		}
	}

	return false
}

func (f *AppearanceFilter) PassesLogFilter(log *Log, addrArray []base.Address) bool {
	haystack := log.Log2Haystack()
	for _, addr := range addrArray {
		if strings.Contains(haystack, addr.Hex()[2:]) {
			return true
		}
	}
	return false
}

func (f *AppearanceFilter) Sort(fromDisc []AppRecord) {
	if f.sortBy == Sorted || f.sortBy == Reversed {
		sort.Slice(fromDisc, func(i, j int) bool {
			if f.sortBy == Reversed {
				i, j = j, i
			}
			si := (base.Blknum(fromDisc[i].BlockNumber) << 32) + base.Blknum(fromDisc[i].TransactionIndex)
			sj := (base.Blknum(fromDisc[j].BlockNumber) << 32) + base.Blknum(fromDisc[j].TransactionIndex)
			return si < sj
		})
	}
}
