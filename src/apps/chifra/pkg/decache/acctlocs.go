package decache

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func LocationsFromAddressEncodingAndBlockIds(conn *rpc.Connection, address base.Address, encoding string, ids []identifiers.Identifier) ([]cache.Locator, error) {
	locations := make([]cache.Locator, 0)
	for _, br := range ids {
		blockNums, err := br.ResolveBlocks(conn.Chain)
		if err != nil {
			return nil, err
		}
		for _, bn := range blockNums {
			// Any type that returns this type of location will work
			locations = append(locations, &types.Result{
				BlockNumber: bn,
				Encoding:    encoding,
				Address:     address,
			})
		}
	}
	return locations, nil
}

func LocationsFromAddrAppsAndCacheType(conn *rpc.Connection, address base.Address, apps []types.Appearance, cT walk.CacheType) ([]cache.Locator, error) {
	locations := make([]cache.Locator, 0)
	for _, app := range apps {
		switch cT {
		case walk.Cache_Slurps:
		case walk.Cache_State:
		case walk.Cache_Tokens:
			logger.Warn("Don't know how to access this cache type", walk.CacheName(cT))
			continue

		case walk.Cache_Logs:
			logGroup := &types.LogGroup{
				BlockNumber:      uint64(app.BlockNumber),
				TransactionIndex: base.NOPOSN,
			}
			locations = append(locations, logGroup)

		case walk.Cache_Blocks:
			locations = append(locations, &types.Block[string]{
				BlockNumber: uint64(app.BlockNumber),
			})

		case walk.Cache_Results:
			locations = append(locations, &types.Result{
				BlockNumber: uint64(app.BlockNumber),
				Encoding:    "*",
				Address:     address,
			})

		case walk.Cache_Statements:
			locations = append(locations, &types.StatementGroup{
				Address:          address,
				BlockNumber:      uint64(app.BlockNumber),
				TransactionIndex: base.Txnum(app.TransactionIndex),
			})

		case walk.Cache_Traces:
			locations = append(locations, &types.TraceGroup{
				BlockNumber:      uint64(app.BlockNumber),
				TransactionIndex: base.Txnum(app.TransactionIndex),
			})

		case walk.Cache_Transactions:
			locations = append(locations, &types.Transaction{
				BlockNumber:      uint64(app.BlockNumber),
				TransactionIndex: base.Txnum(app.TransactionIndex),
			})
		}
	}

	return locations, nil
}
