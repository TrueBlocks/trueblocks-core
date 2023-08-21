package decache

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache/locations"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func Decache(conn *rpc.Connection, locs []cache.Locator, silent bool, cT walk.CacheType) (string, error) {
	itemsSeen := int64(0)
	itemsProcessed := int64(0)
	bytesProcessed := 0

	cacheName := strings.ToLower(cT.String())
	bar := logger.NewBar(logger.BarOptions{
		Prefix:  "Decaching " + cacheName,
		Enabled: !silent, // from opts.Globals.TestMode || len(opts.Globals.File) > 0 because we don't have opts here
		Total:   int64(len(locs)),
	})

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
	return fmt.Sprintf("%d items totaling %d bytes were removed from the %s cache.", itemsProcessed, bytesProcessed, cacheName), nil
}
