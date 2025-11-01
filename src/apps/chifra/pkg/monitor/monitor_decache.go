package monitor

import (
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/decache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/walk"
)

func (mon *Monitor) Decache(conn *rpc.Connection, showProgress bool) (string, error) {
	if apps, cnt, err := mon.ReadAndFilterAppearances(types.NewEmptyFilter(), true /* withCount */); err != nil {
		return "", err
	} else {
		if cnt > 0 {
			monitorCacheTypes := []walk.CacheType{
				// walk.Cache_Statements, see  below
				walk.Cache_Traces,
				walk.Cache_Transactions,
				walk.Cache_Receipts,
				walk.Cache_Withdrawals,
			}
			for _, cacheType := range monitorCacheTypes {
				itemsToRemove, err := decache.LocationsFromAddressAndAppearances(mon.Address, apps, cacheType)
				if err != nil {
					return "", err
				}

				if msg, err := decache.Decache(conn, itemsToRemove, showProgress, cacheType); err != nil {
					return "", err

				} else {
					logger.Progress(showProgress, msg)
				}
			}
		}

		if err := mon.RemoveStatements(len(apps), showProgress); err != nil {
			return "", err
		}

		abiPath := abi.PathToAbisCache(conn.Chain, mon.Address.Hex())
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
	var warning = strings.ReplaceAll("Are sure you want to decache {0}{1} (Yn)?", "{0}", addr)
	if count > 5000 {
		return strings.ReplaceAll(strings.ReplaceAll(warning, "{1}", ". It may take a long time to process {2} records."), "{2}", fmt.Sprintf("%d", count))
	}
	return strings.ReplaceAll(warning, "{1}", "")
}

func (mon *Monitor) RemoveStatements(l int, showProgress bool) error {
	bar := logger.NewBar(logger.BarOptions{
		Prefix:  "Decaching statements if present",
		Enabled: showProgress,
		Total:   int64(l),
	})
	itemsSeen := 0
	visitFunc := func(path string, vP any) (bool, error) {
		_ = vP
		itemsSeen++
		if itemsSeen%1000 == 0 {
			bar.Tick()
		} else {
			bar.Bump()
		}
		if strings.Contains(path, "/"+mon.Address.Hex()[2:]) {
			os.Remove(path)
			// TODO: Clean empty folders here
		}
		return true, nil
	}
	path := walk.GetRootPathFromCacheType(mon.Chain, walk.Cache_Statements)
	err := walk.ForEveryFileInFolder(path, visitFunc, nil)
	bar.Finish(true)
	return err
}
