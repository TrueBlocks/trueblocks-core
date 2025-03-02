package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger2"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// GetTransfers returns a statement from a given transaction
func (r *Reconciler) GetTransfers(pos *types.AppPosition, filter *filter.AppearanceFilter, trans *types.Transaction) ([]types.Transfer, error) {
	if r.connection.Store != nil {
		transferGroup := &types.TransferGroup{
			BlockNumber:      trans.BlockNumber,
			TransactionIndex: trans.TransactionIndex,
		}
		if err := r.connection.Store.Read(transferGroup); err == nil {
			return transferGroup.Transfers, nil
		}
	}

	var err error
	var statements []types.Statement
	if true { // !r.useTraces {
		r2 := ledger2.NewReconciler2(r.connection, r.assetFilter, r.accountFor, r.names, r.asEther)
		if statements, err = r2.GetStatements(pos, filter, trans); err != nil {
			return nil, err
		}
		// } else {
		// 	if statements, err = r.GetStatements(pos, filter, trans); err != nil {
		// 		return nil, err
		// 	}
	}

	if transfers, err := types.ConvertToTransfers(statements); err != nil {
		return nil, err
	} else {

		allReconciled := true
		for _, transfer := range transfers {
			if transfer.IsMaterial() {
				allReconciled = false
				break
			}
		}

		transfersGroup := &types.TransferGroup{
			BlockNumber:      trans.BlockNumber,
			TransactionIndex: trans.TransactionIndex,
			Transfers:        transfers,
		}

		// TODO: BOGUS Turn on caching (remove the false below) for results once we get 100% coverage
		err = r.connection.Store.WriteToCache(transfersGroup, walk.Cache_Transfers, trans.Timestamp, allReconciled, false)
		return transfers, err
	}
}
