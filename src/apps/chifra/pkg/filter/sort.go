package filter

import (
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type AppearanceSort int

const (
	NotSorted AppearanceSort = iota
	Sorted
	Reversed
)

func (f *AppearanceFilter) Sort(fromDisc []types.AppRecord) {
	if f.sortBy == Sorted || f.sortBy == Reversed {
		sort.Slice(fromDisc, func(i, j int) bool {
			if f.sortBy == Reversed {
				i, j = j, i
			}
			si := (uint64(fromDisc[i].BlockNumber) << 32) + uint64(fromDisc[i].TransactionIndex)
			sj := (uint64(fromDisc[j].BlockNumber) << 32) + uint64(fromDisc[j].TransactionIndex)
			return si < sj
		})
	}
}
