package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// GetStatementsFromAppearance visits each appearance in the list and returns a
// list of (hopefully) reconciled statements. Note that if the contexts carried
// balances, this routine could be made concurrent (and reversible).
func (ledgers *Ledger) GetStatementsFromAppearance(chain string, acctFor base.Address, app *types.RawAppearance) (statements []types.SimpleStatement, err error) {
	var tx *types.SimpleTransaction
	if tx, err = rpcClient.GetTransactionByAppearance(chain, app, false); err != nil {
		return []types.SimpleStatement{}, err

	} else {
		if stmts := ledgers.GetStatementsFromTransaction(tx); len(stmts) > 0 {
			for _, s := range stmts {
				if s.Sender == acctFor || s.Recipient == acctFor {
					add := !ledgers.NoZero || len(s.AmountNet().Bits()) != 0
					if add {
						statements = append(statements, *s)
						ledgers.Previous[s.AssetAddr] = s
					}
				}
			}
		} else if err != nil {
			return []types.SimpleStatement{}, err
		}

		return
	}
}
