package ledger

import (
	"fmt"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type appContextKey string

// appContext provides context for a transaction appearance. It tracks the previous,
// current, and next block numbers to help with determining how the balances should be
// reconciled. Additionally, it holds a reconciliation type that describes the differences
// between these block values, and a flag indicating if the ordering is reversed.
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

func (l *Ledger) ctxKey(bn base.Blknum, txid base.Txnum) appContextKey {
	// TODO: Is having the context per asset necessary? Can we use Locator?
	// return fmt.Sprintf("%s-%09d-%05d", l.accountFor.Hex(), bn, txid)
	return appContextKey(fmt.Sprintf("%09d-%05d", bn, txid))
}

const maxTestingBlock = 17000000

// setContexts visits the list of appearances and notes the block numbers of the next and previous
// appearance's and if they are the same or different. Because balances are only available per block,
// we must know this information to be able to calculate the correct post-tx balance.
func (l *Ledger) setContexts(apps []types.Appearance) error {
	for i := 0; i < len(apps); i++ {
		cur := base.Blknum(apps[i].BlockNumber)
		prev := base.Blknum(apps[base.Max(1, i)-1].BlockNumber)
		next := base.Blknum(apps[base.Min(i+1, len(apps)-1)].BlockNumber)
		key := l.ctxKey(base.Blknum(apps[i].BlockNumber), base.Txnum(apps[i].TransactionIndex))
		l.appContexts[key] = newAppContext(base.Blknum(prev), base.Blknum(cur), base.Blknum(next), i == 0, i == (len(apps)-1), l.reversed)
	}
	l.debugContext()
	return nil
}

func (l *Ledger) debugContext() {
	if !l.testMode {
		return
	}

	keys := make([]appContextKey, 0, len(l.appContexts))
	for key := range l.appContexts {
		keys = append(keys, key)
	}

	sort.Slice(keys, func(i, j int) bool {
		return string(keys[i]) < string(keys[j])
	})

	logger.Info(strings.Repeat("-", 60))
	logger.Info(fmt.Sprintf("Contexts (%d)", len(keys)))
	for _, key := range keys {
		c := l.appContexts[key]
		if c.curBlk > maxTestingBlock {
			continue
		}
		msg := ""
		rr := c.reconType &^ (types.First | types.Last)
		switch rr {
		case types.Genesis:
			msg = fmt.Sprintf(" %s", c.reconType.String()+"-diff")
		case types.DiffDiff:
			msg = fmt.Sprintf(" %s", c.reconType.String())
		case types.SameSame:
			msg = fmt.Sprintf(" %s", c.reconType.String())
		case types.DiffSame:
			msg = fmt.Sprintf(" %s", c.reconType.String())
		case types.SameDiff:
			msg = fmt.Sprintf(" %s", c.reconType.String())
		default:
			msg = fmt.Sprintf(" %s should not happen!", c.reconType.String())
		}
		logger.Info(fmt.Sprintf("%s: % 10d % 10d % 11d%s", key, c.prvBlk, c.curBlk, c.nxtBlk, msg))
	}
}
