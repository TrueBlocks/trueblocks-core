package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type appContext struct {
	address   base.Address
	prvBlk    base.Blknum
	curBlk    base.Blknum
	nxtBlk    base.Blknum
	reconType types.ReconType
	reversed  bool
}

func newAppContext(prev, cur, next base.Blknum, isFirst, isLast, reversed bool) *appContext {
	if prev > cur || cur > next {
		return &appContext{reconType: types.Invalid, reversed: reversed}
	}

	reconType := types.Invalid
	if cur == 0 {
		reconType = types.Genesis
	} else {
		prevDiff := prev != cur
		nextDiff := cur != next

		if prevDiff && nextDiff {
			reconType = types.DiffDiff
		} else if !prevDiff && !nextDiff {
			reconType = types.SameSame
		} else if prevDiff {
			reconType = types.DiffSame
		} else if nextDiff {
			reconType = types.SameDiff
		} else {
			reconType = types.Invalid
			logger.Panic("should not happen")
		}
	}

	if isFirst {
		reconType |= types.First
	}

	if isLast {
		reconType |= types.Last
	}

	return &appContext{
		prvBlk:    prev,
		curBlk:    cur,
		nxtBlk:    next,
		reconType: reconType,
		reversed:  reversed,
	}
}

func (c *appContext) Prev() base.Blknum {
	return c.prvBlk
}

func (c *appContext) Cur() base.Blknum {
	return c.curBlk
}

func (c *appContext) Next() base.Blknum {
	return c.nxtBlk
}

func (c *appContext) Recon() types.ReconType {
	return c.reconType
}

func (c *appContext) Address() base.Address {
	return c.address
}
