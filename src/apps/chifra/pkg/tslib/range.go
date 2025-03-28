package tslib

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ranges"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func RangeToBounds(chain string, rng *ranges.FileRange) types.RangeDates {
	firstTs, _ := FromBnToTs(chain, base.Value(rng.First))
	lastTs, _ := FromBnToTs(chain, base.Value(rng.Last))
	ret := types.RangeDates{
		FirstTs:   firstTs,
		FirstDate: base.FormattedDate(firstTs),
		LastTs:    lastTs,
		LastDate:  base.FormattedDate(lastTs),
	}
	return ret
}
