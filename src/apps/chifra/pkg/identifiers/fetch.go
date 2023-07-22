package identifiers

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// TODO: This should be in the rpc package, but identifiers has circular imports.

// FetchTransactionById fetch a transaction given an identifier
func (r *ResolvedId) FetchTransactionById(chain string, fetchTraces bool, rpcOptions *rpcClient.Options) (tx *types.SimpleTransaction, err error) {
	return rpcClient.GetTransactionByAppearance(
		chain,
		&types.RawAppearance{
			BlockNumber:      uint32(r.BlockNumber),
			TransactionIndex: uint32(r.TransactionIndex),
		},
		fetchTraces,
		rpcOptions)
}
