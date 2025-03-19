package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// getUnreconciledStatements returns a list of statements from a given transaction
func (r *Reconciler) getUnreconciledStatements(trans *types.Transaction) ([]*types.Statement, error) {
	var statements []*types.Statement
	if types.IsAssetOfInterest(base.FAKE_ETH_ADDRESS, r.Opts.AssetFilters) {
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
			statements = append(statements, stmt)
		}
	}

	if trans.Receipt != nil {
		if receiptStatements, err := trans.Receipt.FetchStatements(r.Opts.AccountFor, r.Opts.AssetFilters, r.Opts.AppFilters); err != nil {
			return nil, err
		} else {
			for _, stmt := range receiptStatements {
				if stmt.IsMaterial() {
					statements = append(statements, &stmt)
				}
			}
		}
	}

	return statements, nil
}
