package monitorsPkg

import (
	listPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/list"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
)

func (opts *MonitorsOptions) FreshenMonitorsForWatch(addrs []base.Address) (bool, error) {
	strs := make([]string, 0, len(addrs))
	for _, addr := range addrs {
		strs = append(strs, addr.Hex())
	}

	listOpts := listPkg.ListOptions{
		Addrs:   strs,
		Silent:  true,
		Globals: opts.Globals,
	}

	unused := make([]monitor.Monitor, 0, len(addrs))
	return listOpts.HandleFreshenMonitors(&unused)
}
