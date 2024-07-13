package daemonPkg

import "context"

// HandleMonitor starts and manages the monitor process
func (opts *DaemonOptions) HandleMonitor() error {
	_ = context.TODO()
	// monitorOpts := monitorsPkg.GetMonitorsOptions([]string{}, &opts.Globals)
	// monitorOpts.Watch:   true,
	// monitorOpts.Sleep:   opts.Sleep,
	// err, _ := monitorOpts.MonitorsInternal()
	// return err
	return nil
}
