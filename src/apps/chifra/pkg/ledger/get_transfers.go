package ledger

import (
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// GetTransfers returns a statement from a given transaction
func (r *Reconciler) GetTransfers(transactions []*types.Transaction) ([]*types.Transfer, bool, error) {
	items := make([]*types.Transfer, 0)

	for _, tx := range transactions {
		if transfers, err := r.getUnreconciledTransfers(tx); err != nil {
			return nil, false, err
		} else if len(transfers) > 0 {
			items = append(items, transfers...)
		}
	}

	sort.Slice(items, func(i, j int) bool {
		if r.Opts.Reversed {
			i, j = j, i
		}
		if items[i].BlockNumber == items[j].BlockNumber {
			if items[i].TransactionIndex == items[j].TransactionIndex {
				return items[i].LogIndex < items[j].LogIndex
			}
			return items[i].TransactionIndex < items[j].TransactionIndex
		}
		return items[i].BlockNumber < items[j].BlockNumber
	})

	finished := false
	slice := make([]*types.Transfer, 0, len(items))
	for _, item := range items {
		var passes bool
		passes, finished = r.Opts.AppFilters.ApplyCountFilter()
		if passes {
			slice = append(slice, item)
		}
		if finished {
			break
		}
	}

	return slice, finished, nil
}
