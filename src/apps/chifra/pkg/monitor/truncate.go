package monitor

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (m *Monitor) TruncateTo(num uint32) error {
	err := m.ReadMonitorHeader()
	if err != nil {
		return err
	}

	apps := make([]index.AppearanceRecord, m.Count())
	if err = m.ReadAppearances(&apps); err != nil {
		return err
	}

	var keep []index.AppearanceRecord
	for _, app := range apps {
		if app.BlockNumber <= num {
			keep = append(keep, app)
		}
	}
	m.Close() // force the close here so the write is clean

	lastScanned := utils.Min(num, m.Header.LastScanned)
	m.WriteMonHeaderEx(m.Deleted, lastScanned)
	if _, err := m.WriteAppearances(keep, false /* append */); err != nil {
		m.Close()
		return err
	}
	m.Close()

	return nil
}
