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

func Decache(conn *rpc.Connection, locs []cache.Locator, showProgress bool, cT walk.CacheType) (string, error) {
	itemsSeen := int64(0)
	itemsProcessed := int64(0)
	bytesProcessed := 0

	cacheName := strings.ToLower(cT.String())
	bar := logger.NewBar(logger.BarOptions{
		Prefix:  "Decaching " + cacheName + " if present",
		Enabled: showProgress,
		Total:   int64(len(locs)),
	})

	procFunc := func(info *locations.ItemInfo) bool {
		itemsSeen++
		itemsProcessed++
		bytesProcessed += info.Size()
		if itemsSeen%1000 == 0 {
			bar.Tick()
		} else {
			bar.Bump()
		}
		return true
	}

	skipFunc := func(info *locations.ItemInfo) bool {
		itemsSeen++
		if itemsSeen%1000 == 0 {
			bar.Tick()
		} else {
			bar.Bump()
		}
		return true
	}

	if err := conn.Store.Decache(locs, procFunc, skipFunc); err != nil {
		bar.Finish(true /* newLine */)
		return "", err
	}
	bar.Finish(true /* newLine */)
	return fmt.Sprintf("%d items totaling %d bytes were removed from the %s cache.", itemsProcessed, bytesProcessed, cacheName), nil
}
