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

// appContext is a struct to hold the context of a reconciliation (i.e., its
// previous and next blocks and whether they are different)
type appContext struct {
	PrevBlock base.Blknum
	CurBlock  base.Blknum
	NextBlock base.Blknum
	ReconType types.ReconType
}

func newLedgerContext(prev, cur, next base.Blknum, isFirst, isLast, reversed bool) *appContext {
	_ = reversed // Silences unused parameter warning

	if prev > cur || cur > next {
		return &appContext{ReconType: types.Invalid}
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
		PrevBlock: prev,
		CurBlock:  cur,
		NextBlock: next,
		ReconType: reconType,
		// Reversed:  reversed,
	}
}

func (c *appContext) Prev() base.Blknum {
	return c.PrevBlock
}

func (c *appContext) Cur() base.Blknum {
	return c.CurBlock
}

func (c *appContext) Next() base.Blknum {
	return c.NextBlock
}

func (c *appContext) Recon() types.ReconType {
	return c.ReconType
}

func (c *appContext) Address() base.Address {
	return base.ZeroAddr
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
		l.contexts[key] = newLedgerContext(base.Blknum(prev), base.Blknum(cur), base.Blknum(next), i == 0, i == (len(apps)-1), l.reversed)
	}
	l.debugContext()
	return nil
}

func (l *Ledger) debugContext() {
	if !l.testMode {
		return
	}

	keys := make([]appContextKey, 0, len(l.contexts))
	for key := range l.contexts {
		keys = append(keys, key)
	}

	sort.Slice(keys, func(i, j int) bool {
		return string(keys[i]) < string(keys[j])
	})

	logger.Info(strings.Repeat("-", 60))
	logger.Info(fmt.Sprintf("Contexts (%d)", len(keys)))
	for _, key := range keys {
		c := l.contexts[key]
		if c.CurBlock > maxTestingBlock {
			continue
		}
		msg := ""
		rr := c.ReconType &^ (types.First | types.Last)
		switch rr {
		case types.Genesis:
			msg = fmt.Sprintf(" %s", c.ReconType.String()+"-diff")
		case types.DiffDiff:
			msg = fmt.Sprintf(" %s", c.ReconType.String())
		case types.SameSame:
			msg = fmt.Sprintf(" %s", c.ReconType.String())
		case types.DiffSame:
			msg = fmt.Sprintf(" %s", c.ReconType.String())
		case types.SameDiff:
			msg = fmt.Sprintf(" %s", c.ReconType.String())
		default:
			msg = fmt.Sprintf(" %s should not happen!", c.ReconType.String())
		}
		logger.Info(fmt.Sprintf("%s: % 10d % 10d % 11d%s", key, c.PrevBlock, c.CurBlock, c.NextBlock, msg))
	}
}
