package filter

import (
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
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
			si := (base.Blknum(fromDisc[i].BlockNumber) << 32) + base.Blknum(fromDisc[i].TransactionIndex)
			sj := (base.Blknum(fromDisc[j].BlockNumber) << 32) + base.Blknum(fromDisc[j].TransactionIndex)
			return si < sj
		})
	}
}
