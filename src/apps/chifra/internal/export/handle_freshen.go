package exportPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
)

func (opts *ExportOptions) FreshenMonitorsForExport(rCtx output.RenderCtx, monitorArray *[]monitor.Monitor) (bool, error) {
	var updater = monitor.NewUpdater(opts.Globals.Chain, opts.Globals.TestMode, opts.Globals.Decache /* skipFreshen */, opts.Addrs)
	return updater.FreshenMonitors(monitorArray)
}
