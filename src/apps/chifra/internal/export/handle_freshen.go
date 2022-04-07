package exportPkg

import (
	listPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/list"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
)

func (opts *ExportOptions) FreshenMonitors() error {
	listOpts := listPkg.ListOptions{
		Addrs:   opts.Addrs,
		Silent:  true,
		Globals: opts.Globals,
	}

	monitorArray := make([]monitor.Monitor, 0, len(opts.Addrs))
	return listOpts.HandleFreshenMonitors(&monitorArray)
}
