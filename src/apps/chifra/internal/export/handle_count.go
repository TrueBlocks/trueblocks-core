// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/ranges"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
)

func (opts *ExportOptions) HandleCount(rCtx *output.RenderCtx, monitorArray []monitor.Monitor) error {
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		if opts.Globals.Verbose {
			for i := 0; i < len(monitorArray); i++ {
				_ = monitorArray[i].ReadMonitorHeader()
				monitorArray[i].Close()
			}
		}

		testMode := opts.Globals.TestMode
		filter := types.NewFilter(
			opts.Reversed,
			opts.Reverted,
			opts.Fourbytes,
			ranges.BlockRange{First: opts.FirstBlock, Last: opts.LastBlock},
			ranges.RecordRange{First: opts.FirstRecord, Last: opts.GetMax()},
		)

		for _, mon := range monitorArray {
			if apps, cnt, err := mon.ReadAndFilterAppearances(filter, true /* withCount */); err != nil {
				errorChan <- err
				return
			} else if !opts.NoZero || cnt > 0 {
				s := types.Monitor{
					Address:     mon.Address,
					NRecords:    int64(len(apps)),
					FileSize:    file.FileSize(mon.Path()),
					LastScanned: mon.LastScanned,
					Deleted:     mon.Deleted,
				}
				if testMode {
					s.FileSize = 0xdead
					s.LastScanned = maxTestingBlock
				}
				modelChan <- &s
			} else {
				errorChan <- fmt.Errorf("no appearances found for %s", mon.Address.Hex())
				continue
			}
			mon.Close()
		}
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}

const maxTestingBlock = 17000000
