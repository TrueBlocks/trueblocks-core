package ledger

import (
	"fmt"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type ledgerContextKey string

// ledgerContext is a struct to hold the context of a reconciliation (i.e., its
// previous and next blocks and whether they are different)
type ledgerContext struct {
	PrevBlock base.Blknum
	CurBlock  base.Blknum
	NextBlock base.Blknum
	ReconType types.ReconType
}

func newLedgerContext(prev, cur, next base.Blknum, isFirst, isLast, reversed bool) *ledgerContext {
	_ = reversed // Silences unused parameter warning

	if prev > cur || cur > next {
		return &ledgerContext{
			ReconType: types.Invalid,
		}
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

	return &ledgerContext{
		PrevBlock: prev,
		CurBlock:  cur,
		NextBlock: next,
		ReconType: reconType,
		// Reversed:  reversed,
	}
}

func (c *ledgerContext) Prev() base.Blknum {
	return c.PrevBlock
}

func (c *ledgerContext) Cur() base.Blknum {
	return c.CurBlock
}

func (c *ledgerContext) Next() base.Blknum {
	return c.NextBlock
}

func (l *Ledger) ctxKey(bn base.Blknum, txid base.Txnum) ledgerContextKey {
	// TODO: Is having the context per asset necessary? Can we use Locator?
	// return fmt.Sprintf("%s-%09d-%05d", l.AccountFor.Hex(), bn, txid)
	return ledgerContextKey(fmt.Sprintf("%09d-%05d", bn, txid))
}

const maxTestingBlock = 17000000

// SetContexts visits the list of appearances and notes the block numbers of the next and previous
// appearance's and if they are the same or different. Because balances are only available per block,
// we must know this information to be able to calculate the correct post-tx balance.
func (l *Ledger) SetContexts(chain string, apps []types.Appearance) error {
	for i := 0; i < len(apps); i++ {
		cur := base.Blknum(apps[i].BlockNumber)
		prev := base.Blknum(apps[max(1, i)-1].BlockNumber)
		next := base.Blknum(apps[min(i+1, len(apps)-1)].BlockNumber)
		key := l.ctxKey(base.Blknum(apps[i].BlockNumber), base.Txnum(apps[i].TransactionIndex))
		l.Contexts[key] = newLedgerContext(base.Blknum(prev), base.Blknum(cur), base.Blknum(next), i == 0, i == (len(apps)-1), l.Reversed)
	}
	l.debugContext()
	return nil
}

func (l *Ledger) debugContext() {
	if !l.TestMode {
		return
	}

	keys := make([]ledgerContextKey, 0, len(l.Contexts))
	for key := range l.Contexts {
		keys = append(keys, key)
	}

	sort.Slice(keys, func(i, j int) bool {
		return string(keys[i]) < string(keys[j])
	})

	logger.Info(strings.Repeat("-", 60))
	logger.Info(fmt.Sprintf("Contexts (%d)", len(keys)))
	for _, key := range keys {
		c := l.Contexts[key]
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
