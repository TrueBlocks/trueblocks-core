package exportPkg

import (
	listPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/list"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ExportOptions) HandleFreshenMonitors() error {
	listOpts := listPkg.ListOptions{
		Addrs:       opts.Addrs,
		Count:       opts.Count,
		Appearances: opts.Appearances,
		FirstBlock:  opts.FirstBlock,
		LastBlock:   opts.LastBlock,
		Globals:     opts.Globals,
	}

	if listOpts.LastBlock == 0 {
		listOpts.LastBlock = utils.NOPOS
	}

	monitorArray := make([]monitor.Monitor, 0, len(opts.Addrs))
	return listOpts.HandleFreshenMonitors(&monitorArray)
}
