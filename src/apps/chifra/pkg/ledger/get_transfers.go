package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// GetTransfers returns a statement from a given transaction
func (r *Reconciler) GetTransfers(trans *types.Transaction) ([]*types.Transfer, error) {
	return r.getUnreconciledTransfers(trans)
}
