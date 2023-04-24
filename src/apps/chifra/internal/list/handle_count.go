// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package listPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ListOptions) HandleListCount(monitorArray []monitor.Monitor) error {
	testMode := opts.Globals.TestMode
	if opts.Globals.Verbose {
		for i := 0; i < len(monitorArray); i++ {
			monitorArray[i].ReadMonitorHeader()
			monitorArray[i].Close()
		}
	}

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawMonitor], errorChan chan error) {
		for _, mon := range monitorArray {
			if !opts.NoZero || mon.Count() > 0 {
				s := types.SimpleMonitor{
					Address:     mon.Address.Hex(),
					NRecords:    int(mon.Count()),
					FileSize:    file.FileSize(mon.Path()),
					LastScanned: mon.LastScanned,
					Deleted:     mon.Deleted,
				}
				if testMode {
					s.NRecords = 1001001
					s.FileSize = 1001001
					s.LastScanned = maxTestingBlock
				}
				modelChan <- &s
			}
			mon.Close()
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
