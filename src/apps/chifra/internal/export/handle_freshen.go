package exportPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/monitor"
)

func (opts *ExportOptions) FreshenMonitorsForExport(monitorArray *[]monitor.Monitor) (bool, error) {
	var updater = monitor.NewUpdater(opts.Globals.Chain, opts.Globals.TestMode, opts.Globals.Decache /* skipFreshen */, opts.Addrs)
	return updater.FreshenMonitors(monitorArray)
}
