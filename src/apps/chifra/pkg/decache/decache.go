package decache

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache/locations"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
)

func Decache(conn *rpc.Connection, locs []cache.Locator, testMode, verbose bool) (string, error) {
	itemsSeen := int64(0)
	itemsProcessed := int64(0)
	bytesProcessed := 0
	processorFunc := func(info *locations.ItemInfo) bool {
		itemsSeen++
		itemsProcessed++
		bytesProcessed += info.Size()
		logger.Progress(!testMode && itemsProcessed%20 == 0, "Removed", itemsProcessed, "items and", bytesProcessed, "bytes.", info.Name())
		if verbose {
			logger.Info(info.Name(), "was removed.")
		}

		return true
	}

	if err := conn.Store.Decache(locs, processorFunc); err != nil {
		return "", err
	}
	logger.Info("")

	if itemsProcessed == 0 {
		return "No items were removed from the cache.", nil
	} else if itemsProcessed == 1 {
		return fmt.Sprintf("One item totaling %d bytes was removed from the cache.", bytesProcessed), nil
	} else {
		return fmt.Sprintf("%d items totaling %d bytes were removed from the cache.", itemsProcessed, bytesProcessed), nil
	}
}
