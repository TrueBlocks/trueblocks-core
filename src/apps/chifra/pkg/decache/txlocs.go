package decache

import (
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func LocationsFromTransactionIds(conn *rpc.Connection, ids []identifiers.Identifier) ([]cache.Locator, error) {
	locations := make([]cache.Locator, 0)
	for _, rng := range ids {
		txIds, err := rng.ResolveTxs(conn.Chain)
		if err != nil && !errors.Is(err, ethereum.NotFound) {
			continue
		}
		for _, app := range txIds {
			tx := &types.Transaction{
				BlockNumber:      uint64(app.BlockNumber),
				TransactionIndex: uint64(app.TransactionIndex),
			}
			locations = append(locations, tx)
			locations = append(locations, &types.TraceGroup{
				BlockNumber:      tx.BlockNumber,
				TransactionIndex: tx.TransactionIndex,
			})
		}
	}
	return locations, nil
}
