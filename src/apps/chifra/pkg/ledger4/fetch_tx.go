package ledger4

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"

var fetchTx = func(appearance *types.Appearance, withTraces bool) *types.Transaction {
	tx, err := getConnection().GetTransactionByAppearance(appearance, withTraces)
	if err != nil {
		return nil
	}
	return tx
}
