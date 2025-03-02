package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger2"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// GetTransfers returns a statement from a given transaction
func (r *Reconciler) GetTransfers(pos *types.AppPosition, filter *filter.AppearanceFilter, trans *types.Transaction) ([]types.Transfer, error) {
	if r.connection.StoreReadable() {
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
		conn := r.connection
		isFinal := base.IsFinal(r.connection.LatestBlockTimestamp, trans.Timestamp)
		isWritable := conn.StoreWritable()
		isEnabled := conn.EnabledMap[walk.Cache_Transfers]
		// TODO: BOGUS Turn on caching for allTransfers once we get 100% coverage
		if false && isFinal && isWritable && isEnabled && allReconciled {
			transfersGroup := &types.TransferGroup{
				BlockNumber:      trans.BlockNumber,
				TransactionIndex: trans.TransactionIndex,
				Transfers:        transfers,
			}
			_ = conn.Store.Write(transfersGroup)
		}
		return transfers, nil
	}
}
