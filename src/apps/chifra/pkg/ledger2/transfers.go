package ledger2

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func (r *Reconciler2) GetTransfers(pos *types.AppPosition, filter *filter.AppearanceFilter, trans *types.Transaction) ([]types.Transfer, error) {
	if r.connection.StoreReadable() {
		transferGroup := &types.TransferGroup{
			BlockNumber:      trans.BlockNumber,
			TransactionIndex: trans.TransactionIndex,
		}
		if err := r.connection.Store.Read(transferGroup, nil); err == nil {
			return transferGroup.Transfers, nil
		}
	}

	if statements, err := r.GetStatements(pos, filter, trans); err != nil {
		return nil, err
	} else {
		allTransfers := make([]types.Transfer, 0, len(statements)*2)
		for _, stmnt := range statements {
			t := types.Transfer{
				Asset:            stmnt.AssetAddress,
				Holder:           stmnt.AccountedFor,
				Amount:           *stmnt.AmountNet(),
				BlockNumber:      stmnt.BlockNumber,
				TransactionIndex: stmnt.TransactionIndex,
				LogIndex:         stmnt.LogIndex,
				Decimals:         stmnt.Decimals,
			}
			allTransfers = append(allTransfers, t)
		}

		isFinal := base.IsFinal(r.connection.LatestBlockTimestamp, trans.Timestamp)
		isWritable := r.connection.StoreWritable()
		isEnabled := r.connection.EnabledMap[walk.Cache_Transfers]
		// TODO: BOGUS Turn on caching for allTransfers once we get 100% coverage
		if false && isFinal && isWritable && isEnabled {
			for _, transfer := range allTransfers {
				if transfer.IsMaterial() {
					return allTransfers, nil
				}
			}
			transfersGroup := &types.TransferGroup{
				BlockNumber:      trans.BlockNumber,
				TransactionIndex: trans.TransactionIndex,
				Transfers:        allTransfers,
			}
			_ = r.connection.Store.Write(transfersGroup, nil)
		}

		return allTransfers, nil
	}
}
