// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/usage"
)

// HandleDecache handles the command chifra monitors --decache
func (opts *ExportOptions) HandleDecache(monitorArray []monitor.Monitor) error {
	ctx := context.Background()
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
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
