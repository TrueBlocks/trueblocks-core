package monitor

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (m *Monitor) TruncateTo(num uint32) (bool, error) {
	err := m.ReadMonitorHeader()
	if err != nil {
		return false, err
	}

	apps := make([]index.AppearanceRecord, m.Count())
	if err = m.ReadAppearances(&apps); err != nil {
		return false, err
	}

	var keep []index.AppearanceRecord
	for _, app := range apps {
		if app.BlockNumber <= num {
			keep = append(keep, app)
		}
	}
	lastScanned := utils.Min(num, m.Header.LastScanned)

	m.Close() // so when we open it, it gets replaced
	// Very important to note - if you use false for append, the header gets overwritten
	// so ordering matters here and we need to write the header afterwards
	if _, err := m.WriteAppearances(keep, false /* append */); err != nil {
		m.Close()
		return false, err
	}
	m.WriteMonHeader(m.Deleted, lastScanned, true /* force */)
	m.Close()

	return len(apps)-len(keep) > 0, nil
}
