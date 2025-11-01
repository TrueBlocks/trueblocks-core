package decache

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
)

func LocationsFromTraces(conn *rpc.Connection, ids []identifiers.Identifier) ([]cache.Locator, error) {
	locations := make([]cache.Locator, 0)
	for _, br := range ids {
		blockNums, err := br.ResolveBlocks(conn.Chain)
		if err != nil {
			return nil, err
		}
		for _, bn := range blockNums {
			// walk.Cache_Traces
			traceGroup := &types.TraceGroup{
				BlockNumber:      bn,
				TransactionIndex: base.NOPOSN,
			}
			locations = append(locations, traceGroup)
		}
	}
	return locations, nil
}
