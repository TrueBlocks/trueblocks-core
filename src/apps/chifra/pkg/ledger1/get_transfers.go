package ledger1

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger2"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// GetTransfers1 returns a statement from a given transaction
func (r *Reconciler1) GetTransfers1(pos *types.AppPosition, trans *types.Transaction) ([]types.Transfer, error) {
	// statements := make([]types.Statement, 0, 20)
	// if !r.opts.UseTraces {
	// 	stmt := r.getStatementFromTransaction(trans)
	// 	statements = append(statements, *stmt)
	// } else {
	// 	if stmt, err := r.getStatementFromTraces(trans); err == nil {
	// 		statements = append(statements, *stmt)
	// 	}
	// }
	// if trans.Receipt != nil {
	// 	for _, log := range trans.Receipt.Logs {
	// 		if stmt, err := r.getStatementFromLog(&log); err != nil {
	// 			return nil, err
	// 		} else if stmt == nil {
	// 			continue
	// 		} else {
	// 			statements = append(statements, *stmt)
	// 		}
	// 	}
	// }
	// return convertToTransfers(statements)
	r2 := ledger2.NewReconciler(r.opts)
	if statements, err := r2.GetStatements2(pos, trans); err != nil {
		return nil, err
	} else {
		return convertToTransfers(statements)
	}
}

func convertToTransfers(statements []types.Statement) ([]types.Transfer, error) {
	transfers := make([]types.Transfer, 0, len(statements)*2)
	for _, stmnt := range statements {
		t := types.Transfer{
			Asset:            stmnt.Asset,
			Holder:           stmnt.AccountedFor,
			Amount:           *stmnt.AmountNet(),
			BlockNumber:      stmnt.BlockNumber,
			TransactionIndex: stmnt.TransactionIndex,
			LogIndex:         stmnt.LogIndex,
			Decimals:         stmnt.Decimals,
		}
		transfers = append(transfers, t)
	}
	return transfers, nil
}
