package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// GetStatementsFromAppearance visits an appearance and returns a list of (hopefully) reconciled statements.
func (l *Ledger) GetStatementsFromAppearance(chain string, app *types.RawAppearance, rpcOptions *rpcClient.Options) (statements []types.SimpleStatement, err error) {
	var tx *types.SimpleTransaction
	if tx, err = rpcClient.GetTransactionByAppearance(chain, app, false, rpcOptions); err != nil {
		return []types.SimpleStatement{}, err

	} else {
		l.Tx = tx // we need this below
		if stmts := l.GetStatementsFromTransaction(tx); len(stmts) > 0 {
			for _, statement := range stmts {
				statement := statement
				statements = append(statements, *statement)
			}
		} else if err != nil {
			return []types.SimpleStatement{}, err
		}

		return
	}
}
