package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// GetStatementsFromAppearance visits an appearance and returns a list of (hopefully) reconciled statements.
func (ledgers *Ledger) GetStatementsFromAppearance(chain string, app *types.RawAppearance) (statements []types.SimpleStatement, err error) {
	var tx *types.SimpleTransaction
	if tx, err = rpcClient.GetTransactionByAppearance(chain, app, false); err != nil {
		return []types.SimpleStatement{}, err

	} else {
		ledgers.Tx = tx // we need this below
		if stmts := ledgers.GetStatementsFromTransaction(tx); len(stmts) > 0 {
			for _, statement := range stmts {
				passes := false
				if ledgers.AssetFilter != nil && len(*ledgers.AssetFilter) > 0 {
					for _, asset := range *ledgers.AssetFilter {
						if asset == statement.AssetAddr {
							passes = true
							break
						}
					}
				} else {
					passes = true
				}
				add := passes && (!ledgers.NoZero || statement.MoneyMoved())
				if add {
					statement := statement
					statements = append(statements, *statement)
				}
			}
		} else if err != nil {
			return []types.SimpleStatement{}, err
		}

		return
	}
}
