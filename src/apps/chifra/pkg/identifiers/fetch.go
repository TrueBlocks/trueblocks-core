package identifiers

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cacheNew"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// TODO: This should be in the rpc package, but identifiers has circular imports.
// FetchTransactionsById fetch a transaction given an identifier
func (app *ResolvedId) FetchTransactionById(chain string, fetchTraces bool, store *cacheNew.Store) (tx *types.SimpleTransaction, err error) {
	return rpcClient.GetTransactionByAppearance(
		chain,
		&types.RawAppearance{
			BlockNumber:      uint32(app.BlockNumber),
			TransactionIndex: uint32(app.TransactionIndex),
		},
		fetchTraces,
		store)
}
