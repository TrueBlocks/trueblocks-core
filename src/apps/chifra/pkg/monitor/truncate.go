package monitor

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (mon *Monitor) TruncateTo(chain string, num uint32) (bool, error) {
	err := mon.ReadMonitorHeader()
	if err != nil {
		return false, err
	}

	if apps, cnt, err := mon.ReadAndFilterAppearances(filter.NewEmptyFilter(), true /* withCount */); err != nil {
		return false, err

	} else if cnt == 0 {
		return false, nil

	} else {
		var keep []types.SimpleAppRecord
		for _, app := range apps {
			if app.BlockNumber <= num {
				keep = append(keep, types.SimpleAppRecord{
					BlockNumber:      app.BlockNumber,
					TransactionIndex: app.TransactionIndex,
				})
			}
		}
		lastScanned := utils.Min(num, mon.Header.LastScanned)

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
