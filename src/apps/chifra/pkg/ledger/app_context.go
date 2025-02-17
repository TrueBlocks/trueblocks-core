package ledger

import (
	"strings"

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
	address  base.Address
	prvBlk   base.Blknum
	curBlk   base.Blknum
	nxtBlk   base.Blknum
	postType types.PostType
	first    bool
	last     bool
	reversed bool
}

func newAppBalancer(prev, cur, next base.Blknum, isFirst, isLast, reversed bool) *appBalancer {
	if prev > cur || cur > next {
		return &appBalancer{postType: types.Invalid, reversed: reversed}
	}

	postType := types.Invalid
	if cur == 0 {
		postType = types.Genesis
	} else {
		prevDiff := prev != cur
		nextDiff := cur != next
		if prevDiff && nextDiff {
			postType = types.DiffDiff
		} else if !prevDiff && !nextDiff {
			postType = types.SameSame
		} else if prevDiff {
			postType = types.DiffSame
		} else if nextDiff {
			postType = types.SameDiff
		} else {
			postType = types.Invalid
			logger.Panic("should not happen")
		}
	}

	return &appBalancer{
		prvBlk:   prev,
		curBlk:   cur,
		nxtBlk:   next,
		postType: postType,
		first:    isFirst,
		last:     isLast,
		reversed: reversed,
	}
}

func (c *appBalancer) Prev() base.Blknum {
	if c.first {
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
	if c.last {
		return c.nxtBlk + 1
	}
	return c.nxtBlk
}

func (c *appBalancer) PostType() types.PostType {
	return c.postType
}

func (c *appBalancer) ReconStr() string {
	ret := c.postType.String()
	if strings.Contains(ret, "genesis") || !strings.Contains(ret, "-") {
		return ret
	}

	parts := strings.Split(ret, "-")
	if c.first {
		parts[0] = "first"
	}
	if c.last {
		parts[1] = "last"
	}
	return strings.Join(parts, "-")
}

func (c *appBalancer) Address() base.Address {
	return c.address
}

func (c *appBalancer) RunningBal() *base.Wei {
	return nil
}
