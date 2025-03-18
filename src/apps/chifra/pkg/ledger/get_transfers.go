package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// GetTransfers returns a statement from a given transaction
func (r *Reconciler) GetTransfers(trans *types.Transaction) ([]types.Transfer, error) {
	if statements, err := r.getUnreconciledStatements(trans); err != nil {
		return nil, err
	} else {
		transfers := make([]types.Transfer, 0, len(statements)*2)
		for _, stmnt := range statements {
			t := types.Transfer{
				Asset:            stmnt.Asset,
				Holder:           stmnt.AccountedFor,
				Amount:           *stmnt.AmountNet(),
				BlockNumber:      stmnt.BlockNumber,
				TransactionIndex: stmnt.TransactionIndex,
				LogIndex:         stmnt.LogIndex,
				Decimals:         uint64(stmnt.Decimals),
			}
			if !t.Amount.Equal(base.ZeroWei) {
				transfers = append(transfers, t)
			}
		}
		return transfers, nil
	}
}
