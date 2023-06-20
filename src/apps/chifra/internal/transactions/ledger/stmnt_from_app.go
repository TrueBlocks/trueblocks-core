package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// GetStatementsFromAppearance visits an appearance and returns a list of (hopefully) reconciled statements.
func (l *Ledger) GetStatementsFromAppearance(chain string, app *types.RawAppearance) (statements []types.SimpleStatement, err error) {
	var tx *types.SimpleTransaction
	if tx, err = rpcClient.GetTransactionByAppearance(chain, app, false); err != nil {
		return []types.SimpleStatement{}, err

	} else {
		if stmts := l.GetStatementsFromTransaction(tx); len(stmts) > 0 {
			for _, s := range stmts {
				add := !l.NoZero || len(s.AmountNet().Bits()) != 0
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
