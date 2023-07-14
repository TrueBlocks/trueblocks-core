package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// GetStatementsFromAppearance visits an appearance and returns a list of (hopefully) reconciled statements.
func (ledgers *Ledger) GetStatementsFromAppearance(chain string, app *types.RawAppearance) (statements []types.SimpleStatement, err error) {
	var tx *types.SimpleTransaction
	if tx, err = rpcClient.GetTransactionByAppearance(chain, app, false, nil); err != nil {
		return []types.SimpleStatement{}, err

	} else {
		ledgers.Tx = tx // we need this below
		if stmts := ledgers.GetStatementsFromTransaction(tx); len(stmts) > 0 {
			for _, s := range stmts {
				add := !ledgers.NoZero || s.MoneyMoved()
				if add {
					statements = append(statements, *s)
				}
			}
		} else if err != nil {
			return []types.SimpleStatement{}, err
		}

		return
	}
}
