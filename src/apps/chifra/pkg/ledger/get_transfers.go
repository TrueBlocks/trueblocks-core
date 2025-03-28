package ledger

import (
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// -----------------------------------------------------------------
func (r *Reconciler) GetTransfers(txs []*types.Transaction) ([]*types.Transfer, bool, error) {
	ethTransfers, tokenTransfers, _, err := r.getTransfersInternal(txs, false)
	if err != nil {
		return nil, false, err
	}

	transfers := append(ethTransfers, tokenTransfers...)
	sort.Slice(transfers, func(i, j int) bool {
		if r.Opts.Reversed {
			i, j = j, i
		}
		if transfers[i].BlockNumber == transfers[j].BlockNumber {
			if transfers[i].TransactionIndex == transfers[j].TransactionIndex {
				return transfers[i].LogIndex < transfers[j].LogIndex
			}
			return transfers[i].TransactionIndex < transfers[j].TransactionIndex
		}
		return transfers[i].BlockNumber < transfers[j].BlockNumber
	})

	finished := false
	slice := make([]*types.Transfer, 0, len(transfers))
	for _, item := range transfers {
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
