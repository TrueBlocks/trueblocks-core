package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// GetStatementsFromAppearance visits an appearance and returns a list of (hopefully) reconciled statements.
func (l *Ledger) GetStatementsFromAppearance(conn *rpc.Connection, app *types.RawAppearance) (statements []types.SimpleStatement, err error) {
	var tx *types.SimpleTransaction
	if tx, err = conn.GetTransactionByAppearance(app, false); err != nil {
		return []types.SimpleStatement{}, err

	} else {
		l.Tx = tx // we need this below
		if stmts := l.GetStatementsFromTransaction(conn, tx); len(stmts) > 0 {
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
