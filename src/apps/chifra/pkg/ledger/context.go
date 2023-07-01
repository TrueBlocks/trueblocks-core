package ledger

import (
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/ethereum/go-ethereum"
)

// Context is a struct to hold the context of a reconciliation (i.e., its previous and next blocks and whether they are different)
type LedgerContext struct {
	PrevBlock  base.Blknum
	CurBlock   base.Blknum
	NextBlock  base.Blknum
	IsPrevDiff bool
	IsNextDiff bool
	ReconType  string
}

func (l *Ledger) CtxKey(bn, txid uint64) string {
	return fmt.Sprintf("%s-%09d-%05d", l.AccountFor.Hex(), bn, txid)
}

// SetContexts visits the list of appearances and notes the block numbers of the next and previous
// appearance's and if they are the same or different. Because balances are only available per block,
// we must know this information to be able to calculate the correct post-tx balance.
func (l *Ledger) SetContexts(chain string, apps []index.AppearanceRecord) error {
	for i := 0; i < len(apps); i++ {
		cur := apps[i].BlockNumber

		prev := uint32(0)
		if i > 0 {
			prev = apps[i-1].BlockNumber
		}

		next := uint32(^uint32(0))
		if i < len(apps)-1 {
			next = apps[i+1].BlockNumber
		}

		key := l.CtxKey(uint64(apps[i].BlockNumber), uint64(apps[i].TransactionId))
		l.Contexts[key] = LedgerContext{
			PrevBlock:  base.Blknum(prev),
			CurBlock:   base.Blknum(cur),
			NextBlock:  base.Blknum(next),
			IsPrevDiff: prev != cur,
			IsNextDiff: cur != next,
			ReconType:  getReconType(cur, prev != cur, cur != next),
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

			key := l.CtxKey(uint64(apps[i].BlockNumber), uint64(apps[i].TransactionIndex))
			l.Contexts[key] = LedgerContext{
				PrevBlock:  base.Blknum(prev),
				CurBlock:   base.Blknum(cur),
				NextBlock:  base.Blknum(next),
				IsPrevDiff: prev != cur,
				IsNextDiff: cur != next,
				ReconType:  getReconType(cur, prev != cur, cur != next),
			}
		}
	}
	return nil
}

func getReconType(bn uint32, isPrevDiff bool, isNextDiff bool) (reconType string) {
	if bn == 0 {
		return "genesis"
	}

	if isPrevDiff && isNextDiff {
		return "regular"
	} else if !isPrevDiff && !isNextDiff {
		return "both-not-diff"
	} else if isPrevDiff {
		return "prevDiff"
	} else if isNextDiff {
		return "nextDiff"
	} else {
		return "should-not-happen"
	}
}
