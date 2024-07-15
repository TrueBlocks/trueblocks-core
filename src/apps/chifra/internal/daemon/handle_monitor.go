package daemonPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
)

// HandleMonitor starts and manages the monitor process
func (opts *DaemonOptions) HandleMonitor(rCtx *output.RenderCtx) error {
	// monitorOpts := monitorsPkg.GetMonitorsOptions([]string{}, &opts.Globals)
	// monitorOpts.Watch:   true,
	// monitorOpts.Sleep:   opts.Sleep,
	// err, _ := monitorOpts.MonitorsInternal()
	// return err
	return nil
}
