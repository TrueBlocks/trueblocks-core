package decache

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache/locations"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
)

func Decache(conn *rpc.Connection, locs []cache.Locator, silent bool, reason string) (string, error) {
	itemsSeen := int64(0)
	itemsProcessed := int64(0)
	bytesProcessed := 0

	bar := logger.NewBar("Decaching "+reason, !silent, int64(len(locs)))
	processorFunc := func(info *locations.ItemInfo) bool {
		itemsSeen++
		itemsProcessed++
		bytesProcessed += info.Size()
		bar.Tick()
		return true
	}

	if err := conn.Store.Decache(locs, processorFunc); err != nil {
		bar.Finish(true)
		return "", err
	}
	bar.Finish(true)
	return fmt.Sprintf("%d items totaling %d bytes were removed from the cache.", itemsProcessed, bytesProcessed), nil
}
