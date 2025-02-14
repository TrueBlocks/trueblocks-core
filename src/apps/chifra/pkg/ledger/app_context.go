package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type appBalancerKey string

// appBalancer provides context for a transaction appearance. It tracks the previous,
// current, and next block numbers to help with determining how the balances should be
// reconciled. Additionally, it holds a reconciliation type that describes the differences
// between these block values, and a flag indicating if the ordering is reversed.
type appBalancer struct {
	address   base.Address
	prvBlk    base.Blknum
	curBlk    base.Blknum
	nxtBlk    base.Blknum
	reconType types.ReconType
	reversed  bool
}

func newAppBalancer(prev, cur, next base.Blknum, isFirst, isLast, reversed bool) *appBalancer {
	if prev > cur || cur > next {
		return &appBalancer{reconType: types.Invalid, reversed: reversed}
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

	return &appBalancer{
		prvBlk:    prev,
		curBlk:    cur,
		nxtBlk:    next,
		reconType: reconType,
		reversed:  reversed,
	}
}

func (c *appBalancer) Prev() base.Blknum {
	if c.reconType&types.First != 0 {
		if c.prvBlk == 0 {
			return 0
		}
		return c.prvBlk - 1
	}
	return c.prvBlk
}

func (c *appBalancer) Cur() base.Blknum {
	return c.curBlk
}

func (c *appBalancer) Next() base.Blknum {
	if c.reconType&types.Last != 0 {
		return c.nxtBlk + 1
	}
	return c.nxtBlk
}

func (c *appBalancer) Recon() types.ReconType {
	return c.reconType
}

func (c *appBalancer) Address() base.Address {
	return c.address
}

func (c *appBalancer) RunningBal() *base.Wei {
	return nil
}
