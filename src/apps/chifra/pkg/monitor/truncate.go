package monitor

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (mon *Monitor) TruncateTo(chain string, num uint32) (bool, error) {
	_ = chain
	err := mon.ReadMonitorHeader()
	if err != nil {
		return false, err
	}

	if apps, cnt, err := mon.ReadAndFilterAppearances(filter.NewEmptyFilter(), true /* withCount */); err != nil {
		return false, err

	} else if cnt == 0 {
		return false, nil

	} else {
		var keep []types.AppRecord
		for _, app := range apps {
			if app.BlockNumber <= num {
				keep = append(keep, types.AppRecord{
					BlockNumber:      app.BlockNumber,
					TransactionIndex: app.TransactionIndex,
				})
			}
		}
		lastScanned := base.Min(num, mon.Header.LastScanned)

		mon.Close() // so when we open it, it gets replaced
		// Very important to note - if you use false for append, the header gets overwritten
		// so ordering matters here and we need to write the header afterwards
		if _, err := mon.WriteAppearances(keep, false /* append */); err != nil {
			mon.Close()
			return false, err
		}
		_ = mon.WriteMonHeader(mon.Deleted, lastScanned, true /* force */)
		mon.Close()

		return len(apps)-len(keep) > 0, nil
	}
}
