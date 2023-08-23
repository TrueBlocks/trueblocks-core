package ledger

import (
	"errors"
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

// LedgerContext is a struct to hold the context of a reconciliation (i.e., its
// previous and next blocks and whether they are different)
type LedgerContext struct {
	PrevBlock  base.Blknum
	CurBlock   base.Blknum
	NextBlock  base.Blknum
	IsPrevDiff bool
	IsNextDiff bool
	ReconType  string
}

func (c *LedgerContext) Prev() base.Blknum {
	return c.PrevBlock
}

func (c *LedgerContext) Cur() base.Blknum {
	return c.CurBlock
}

func (c *LedgerContext) Next() base.Blknum {
	return c.NextBlock
}

func NewLedgerContext(prev, cur, next base.Blknum) *LedgerContext {
	c := &LedgerContext{
		PrevBlock:  prev,
		CurBlock:   cur,
		NextBlock:  next,
		IsPrevDiff: prev != cur,
		IsNextDiff: cur != next,
	}
	c.ReconType = c.getReconType()
	return c
}

func (c *LedgerContext) getReconType() (reconType string) {
	if c.CurBlock == 0 {
		c.IsPrevDiff = true
		return "genesis"
	} else {
		if c.IsPrevDiff && c.IsNextDiff {
			return "diff-diff"
		} else if !c.IsPrevDiff && !c.IsNextDiff {
			return "same-same"
		} else if c.IsPrevDiff {
			return "diff-same"
		} else if c.IsNextDiff {
			return "same-diff"
		} else {
			return "should-not-happen"
		}
	}
}

func (l *Ledger) ctxKey(bn, txid uint64) string {
	// return fmt.Sprintf("%s-%09d-%05d", l.AccountFor.Hex(), bn, txid)
	return fmt.Sprintf("%09d-%05d", bn, txid)
}

const maxTestingBlock = 17000000

// SetContexts visits the list of appearances and notes the block numbers of the next and previous
// appearance's and if they are the same or different. Because balances are only available per block,
// we must know this information to be able to calculate the correct post-tx balance.
func (l *Ledger) SetContexts(chain string, apps []types.SimpleAppearance, outerBounds base.BlockRange) error {
	for i := 0; i < len(apps); i++ {
		cur := apps[i].BlockNumber
		if cur > maxTestingBlock {
			continue
		}

		prev := outerBounds.First
		if i > 0 {
			prev = uint64(apps[i-1].BlockNumber)
		}

		next := outerBounds.Last
		if i < len(apps)-1 {
			next = uint64(apps[i+1].BlockNumber)
		}

		if next < l.FirstBlock || prev > l.LastBlock {
			continue
		}

		key := l.ctxKey(uint64(apps[i].BlockNumber), uint64(apps[i].TransactionIndex))
		l.Contexts[key] = *NewLedgerContext(base.Blknum(prev), base.Blknum(cur), base.Blknum(next))
	}

	if l.TestMode {
		keys := []string{}
		for key := range l.Contexts {
			keys = append(keys, key)
		}
		sort.Strings(keys)
		for _, key := range keys {
			c := l.Contexts[key]
			if c.CurBlock > maxTestingBlock {
				continue
			}
			msg := ""
			if !c.IsPrevDiff || !c.IsNextDiff {
				msg = fmt.Sprintf(" %12.12s %t %t", c.ReconType, c.IsPrevDiff, c.IsNextDiff)
			}
			logger.Info(fmt.Sprintf("%s: % 10d % 10d % 11d%s", key, c.PrevBlock, c.CurBlock, c.NextBlock, msg))
		}
	}

	return nil
}

// SetContextsFromIds produces reconciliation contexts for a list of transaction id that
// most probably won't reconicile since there will be missing gaps in the list. (For example,
// from chifra transactions --account_for 10 100 1000.)
func (l *Ledger) SetContextsFromIds(chain string, txIds []identifiers.Identifier) error {
	for _, rng := range txIds {
		apps, err := rng.ResolveTxs(chain)
		if err != nil && !errors.Is(err, ethereum.NotFound) {
			return err
		}
		for i := 0; i < len(apps); i++ {
			cur := apps[i].BlockNumber
			prev := uint32(0)
			if apps[i].BlockNumber > 0 {
				prev = apps[i].BlockNumber - 1
			}

			next := apps[i].BlockNumber + 1

			key := l.ctxKey(uint64(apps[i].BlockNumber), uint64(apps[i].TransactionIndex))
			l.Contexts[key] = *NewLedgerContext(base.Blknum(prev), base.Blknum(cur), base.Blknum(next))
		}
	}
	return nil
}
