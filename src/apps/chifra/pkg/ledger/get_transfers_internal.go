package ledger

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"

func (r *Reconciler) getTransfersInternal(txs []*types.Transaction) ([]*types.Transfer, []*types.Transfer, error) {
	ethTransfers := make([]*types.Transfer, 0)
	tokenTransfers := make([]*types.Transfer, 0)
	for _, tx := range txs {
		if ethXfers, tokenXfers, err := r.getUnreconciledTransfers(tx); err != nil {
			return nil, nil, err
		} else {
			ethTransfers = append(ethTransfers, ethXfers...)
			tokenTransfers = append(tokenTransfers, tokenXfers...)
		}
	}
	return ethTransfers, tokenTransfers, nil
}
