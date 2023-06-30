package ledger

import (
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
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

// SetContexts visits the list of appearances and notes the block numbers of the next and previous
// appearance's and if they are the same or different. Because balances are only available per block,
// we must know this information to be able to calculate the correct post-tx balance.
func (l *Ledger) SetContexts(chain string, txIds []identifiers.Identifier) error {
	for _, rng := range txIds {
		apps, err := rng.ResolveTxs(chain)
		if err != nil && !errors.Is(err, ethereum.NotFound) {
			return err
		}
		for i := 0; i < len(apps); i++ {
			prev := uint32(0)
			if apps[i].BlockNumber > 0 {
				prev = apps[i].BlockNumber - 1
			}
			next := apps[i].BlockNumber + 1
			cur := apps[i].BlockNumber
			getReconType := func(isPrevDiff bool, isNextDiff bool) (reconType string) {
				if isPrevDiff && isNextDiff {
					return "regular"
				} else if !isPrevDiff && !isNextDiff {
					return "both-not-diff"
				} else if isPrevDiff {
					return "prevDiff"
				} else if isNextDiff {
					return "nextDiff"
				} else {
					return "unknown"
				}
			}
			ctext := LedgerContext{
				PrevBlock:  base.Blknum(prev),
				CurBlock:   base.Blknum(cur),
				NextBlock:  base.Blknum(next),
				IsPrevDiff: prev != cur,
				IsNextDiff: cur != next,
				ReconType:  getReconType(prev != cur, cur != next),
			}
			key := fmt.Sprintf("%09d-%05d", apps[i].BlockNumber, apps[i].TransactionIndex)
			l.Contexts[key] = ctext
		}
	}
	return nil
}
