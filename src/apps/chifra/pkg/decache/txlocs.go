package decache

import (
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func TransactionLocationsFromIds(conn *rpcClient.Connection, ids []identifiers.Identifier) ([]cache.Locator, error) {
	locations := make([]cache.Locator, 0)
	for _, rng := range ids {
		txIds, err := rng.ResolveTxs(conn.Chain)
		if err != nil && !errors.Is(err, ethereum.NotFound) {
			continue
		}
		for _, app := range txIds {
			tx := &types.SimpleTransaction{
				BlockNumber:      uint64(app.BlockNumber),
				TransactionIndex: uint64(app.TransactionIndex),
			}
			locations = append(locations, tx)
			locations = append(locations, &types.SimpleTraceGroup{
				BlockNumber:      tx.BlockNumber,
				TransactionIndex: tx.TransactionIndex,
			})
		}
	}
	return locations, nil
}
