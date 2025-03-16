package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// GetTransfers returns a statement from a given transaction
func (r *Reconciler) GetTransfers(trans *types.Transaction) ([]types.Transfer, error) {
	var statements []types.Statement
	if types.AssetOfInterest(r.Opts.AssetFilters, base.FAKE_ETH_ADDRESS) {
		var stmt *types.Statement
		if r.Opts.UseTraces {
			if traces, err := r.Connection.GetTracesByTransactionHash(trans.Hash.Hex(), trans); err != nil {
				return nil, err
			} else {
				if s, err := trans.FetchStatementFromTraces(traces, r.Opts.AccountFor, r.Opts.AsEther); err != nil {
					return nil, err
				} else {
					stmt = s
				}
			}
		} else {
			var err error
			if stmt, err = trans.FetchStatement(r.Opts.AsEther, base.FAKE_ETH_ADDRESS, r.Opts.AccountFor); err != nil {
				return nil, err
			}
		}
		// Append only if the statement is material
		if stmt.IsMaterial() {
			statements = append(statements, *stmt)
		}
	}

	if trans.Receipt != nil {
		if receiptStatements, err := trans.Receipt.FetchStatements(r.Opts.AccountFor, r.Opts.AssetFilters, r.Opts.AppFilters); err != nil {
			return nil, err
		} else {
			for _, stmt := range receiptStatements {
				if stmt.IsMaterial() {
					statements = append(statements, stmt)
				}
			}
		}
	}

	return convertToTransfers(statements)
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
		if !t.Amount.Equal(base.ZeroWei) {
			transfers = append(transfers, t)
		}
	}
	return transfers, nil
}
