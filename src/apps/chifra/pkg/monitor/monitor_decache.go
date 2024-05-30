package monitor

import (
	"fmt"
	"os"
	"path"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/decache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func (mon *Monitor) Decache(conn *rpc.Connection, showProgress bool) (string, error) {
	if apps, cnt, err := mon.ReadAndFilterAppearances(filter.NewEmptyFilter(), true /* withCount */); err != nil {
		return "", err
	} else {
		if cnt > 0 {
			caches := []walk.CacheType{
				walk.Cache_Statements,
				walk.Cache_Traces,
				walk.Cache_Transactions,
				walk.Cache_Receipts,
				walk.Cache_Withdrawals,
			}
			for _, cache := range caches {
				itemsToRemove, err := decache.LocationsFromAddrAppsAndCacheType(conn, mon.Address, apps, cache)
				if err != nil {
					return "", err
				}

				if msg, err := decache.Decache(conn, itemsToRemove, showProgress, cache); err != nil {
					return "", err

				} else {
					logger.Progress(showProgress, msg)
				}
			}
		}

		abiPath := path.Join(walk.CacheTypeToFolder[walk.Cache_Abis], mon.Address.Hex()+".json")
		if file.FileExists(abiPath) {
			os.Remove(abiPath)
			logger.Progress(showProgress, "Abi "+abiPath+" file removed.")
		}

		return mon.RemoveMonitor(), nil
	}
}

func (mon *Monitor) RemoveMonitor() string {
	clear := strings.Repeat(" ", 30)
	mon.Close()
	mon.Delete()
	if wasRemoved, err := mon.Remove(); !wasRemoved || err != nil {
		return fmt.Sprintf("Monitor for %s was not removed (%s).%s", mon.Address.Hex(), err.Error(), clear)
	} else {
		return fmt.Sprintf("Monitor for %s was permanently removed.%s", mon.Address.Hex(), clear)
	}
}

func (mon *Monitor) GetRemoveWarning() string {
	addr := mon.Address.Hex()
	count := mon.Count()
	var warning = strings.Replace("Are sure you want to decache {0}{1} (Yn)?", "{0}", addr, -1)
	if count > 5000 {
		return strings.Replace(strings.Replace(warning, "{1}", ". It may take a long time to process {2} records.", -1), "{2}", fmt.Sprintf("%d", count), -1)
	}
	return strings.Replace(warning, "{1}", "", -1)
}
