package exportPkg

import (
	listPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/list"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
)

func (opts *ExportOptions) FreshenMonitorsForExport(monitorArray *[]monitor.Monitor) (bool, error) {
	listOpts := listPkg.ListOptions{
		Addrs:   opts.Addrs,
		Silent:  true,
		Globals: opts.Globals,
	}

	return listOpts.HandleFreshenMonitors(monitorArray)
}
