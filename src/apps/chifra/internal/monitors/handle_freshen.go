package monitorsPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/monitor"
)

func (opts *MonitorsOptions) FreshenMonitorsForWatch(addrs []base.Address) (bool, error) {
	strs := make([]string, 0, len(addrs))
	for _, addr := range addrs {
		strs = append(strs, addr.Hex())
	}

	unusedMonitors := make([]monitor.Monitor, 0, len(addrs))
	var updater = monitor.NewUpdater(opts.Globals.Chain, opts.Globals.TestMode, false /* skipFreshen */, strs)
	return updater.FreshenMonitors(&unusedMonitors)
}
