package listPkg

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"

func (opts *ListOptions) FreshenMonitorsForList(monitorArray *[]monitor.Monitor) (bool, error) {
	var updater = monitor.NewUpdater(opts.Globals.Chain, opts.Globals.TestMode, opts.Addrs)
	updater.PublisherAddr = opts.PublisherAddr
	return updater.FreshenMonitors(monitorArray)
}
