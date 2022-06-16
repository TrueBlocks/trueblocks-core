package monitorsPkg

import (
	listPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/list"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
)

func (opts *MonitorsOptions) FreshenMonitorsScrape(addrs []string) error {
	listOpts := listPkg.ListOptions{
		Addrs:   addrs,
		Silent:  true,
		Globals: opts.Globals,
	}

	unused := make([]monitor.Monitor, 0, len(addrs))
	return listOpts.HandleFreshenMonitors(&unused)
}
