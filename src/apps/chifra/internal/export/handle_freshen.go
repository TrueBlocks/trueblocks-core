package exportPkg

import (
	listPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/list"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ExportOptions) HandleFreshenMonitors() error {
	listOpts := listPkg.ListOptions{}
	listOpts.Addrs = opts.Addrs
	listOpts.Count = opts.Count
	listOpts.Appearances = opts.Appearances
	listOpts.FirstBlock = opts.FirstBlock
	listOpts.LastBlock = opts.LastBlock
	if listOpts.LastBlock == 0 {
		listOpts.LastBlock = utils.NOPOS
	}
	listOpts.Globals = opts.Globals
	monitorArray := make([]monitor.Monitor, 0, len(opts.Addrs))
	return listOpts.HandleFreshenMonitors(&monitorArray)
}
