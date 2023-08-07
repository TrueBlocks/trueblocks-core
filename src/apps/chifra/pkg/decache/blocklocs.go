package decache

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func BlockLocationsFromIds(conn *rpc.Connection, ids []identifiers.Identifier) ([]cache.Locator, error) {
	locations := make([]cache.Locator, 0)
	for _, br := range ids {
		blockNums, err := br.ResolveBlocks(conn.Chain)
		if err != nil {
			return nil, err
		}
		for _, bn := range blockNums {
			rawBlock, err := conn.GetBlockHeaderByNumber(bn)
			if err != nil {
				return nil, err
			}
			locations = append(locations, &types.SimpleBlock[string]{
				BlockNumber: bn,
			})
			for index := range rawBlock.Transactions {
				txToRemove := &types.SimpleTransaction{
					BlockNumber:      bn,
					TransactionIndex: uint64(index),
				}
				locations = append(locations, txToRemove)
				locations = append(locations, &types.SimpleTraceGroup{
					BlockNumber:      bn,
					TransactionIndex: base.Txnum(index),
				})
			}
		}
	}
	return locations, nil
}
