package monitorsPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/usage"
)

func (opts *MonitorsOptions) HandleDecache(rCtx *output.RenderCtx) error {
	monitorArray := make([]monitor.Monitor, 0, len(opts.Addrs))
	var updater = monitor.NewUpdater(opts.Globals.Chain, opts.Globals.TestMode, true /* skipFreshen */, opts.Addrs)
	if canceled, err := updater.FreshenMonitors(&monitorArray); err != nil || canceled {
		return err
	}

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		doIt := true
		for _, mon := range monitorArray {
			if doIt && !opts.Globals.IsApiMode() && !usage.QueryUser(mon.GetRemoveWarning(), "Not decaching") {
				continue
			}
			doIt = false
			showProgress := opts.Globals.ShowProgressNotTesting()
			if result, err := mon.Decache(opts.Conn, showProgress); err != nil {
				errorChan <- err
				continue
			} else {
				modelChan <- &types.Message{
					Msg: result,
				}
			}
		}
	}
	opts.Globals.NoHeader = true
	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}
