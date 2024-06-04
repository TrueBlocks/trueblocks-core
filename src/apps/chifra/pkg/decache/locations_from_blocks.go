package decache

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func LocationsFromBlocks(conn *rpc.Connection, ids []identifiers.Identifier) ([]cache.Locator, error) {
	locations := make([]cache.Locator, 0)
	for _, br := range ids {
		blockNums, err := br.ResolveBlocks(conn.Chain)
		if err != nil {
			return nil, err
		}
		for _, bn := range blockNums {
			block, err := conn.GetBlockHeaderByNumber(bn)
			if err != nil {
				return nil, err
			}
			// walk.Cache_Blocks
			locations = append(locations, &types.LightBlock{
				BlockNumber: bn,
			})

			// walk.Cache_Receipts
			receiptGroup := &types.ReceiptGroup{
				BlockNumber:      bn,
				TransactionIndex: base.NOPOSN,
			}
			locations = append(locations, receiptGroup)

			for index := range block.Transactions {
				// walk.Cache_Transactions
				locations = append(locations, &types.Transaction{
					BlockNumber:      bn,
					TransactionIndex: base.Txnum(index),
				})

				// walk.Cache_Traces
				locations = append(locations, &types.TraceGroup{
					BlockNumber:      bn,
					TransactionIndex: base.Txnum(index),
				})
			}
		}
	}
	return locations, nil
}
