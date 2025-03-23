package decache

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func LocationsFromAddressAndAppearances(address base.Address, apps []types.Appearance, cT walk.CacheType) ([]cache.Locator, error) {
	_ = address
	locations := make([]cache.Locator, 0)
	for _, app := range apps {
		bn := base.Blknum(app.BlockNumber)
		txid := base.Txnum(app.TransactionIndex)
		switch cT {
		case walk.Cache_Transactions:
			locations = append(locations, &types.Transaction{
				BlockNumber:      bn,
				TransactionIndex: txid,
			})
		case walk.Cache_Receipts:
			locations = append(locations, &types.Transaction{
				BlockNumber:      bn,
				TransactionIndex: txid,
			})
		case walk.Cache_Traces:
			locations = append(locations, &types.TraceGroup{
				BlockNumber:      bn,
				TransactionIndex: txid,
			})
		case walk.Cache_Withdrawals:
			locations = append(locations, &types.LightBlock{
				BlockNumber: bn,
			})
		case walk.Cache_Statements:
			locations = append(locations, &types.StatementGroup{
				Address:          address,
				BlockNumber:      bn,
				TransactionIndex: txid,
			})
		case walk.Cache_Transfers:
			locations = append(locations, &types.TransferGroup{
				Address:          address,
				BlockNumber:      bn,
				TransactionIndex: txid,
			})
		}
	}

	return locations, nil
}
