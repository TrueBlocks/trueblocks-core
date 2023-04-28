// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package listPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ListOptions) HandleListCount(monitorArray []monitor.Monitor) error {
	testMode := opts.Globals.TestMode
	if opts.Globals.Verbose {
		for i := 0; i < len(monitorArray); i++ {
			monitorArray[i].ReadMonitorHeader()
			monitorArray[i].Close()
		}
	}

	exportRange := opts.minMax()

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawMonitor], errorChan chan error) {
		for _, mon := range monitorArray {
			if !opts.NoZero || mon.Count() > 0 {
				mon.Close()
				apps := make([]index.AppearanceRecord, mon.Count())
				if err := mon.ReadAppearances(&apps); err != nil {
					errorChan <- err
					continue
				}

				nRecords := 0
				for _, appearance := range apps {
					if exportRange.IntersectsB(uint64(appearance.BlockNumber)) {
						nRecords++
					}
				}

				s := types.SimpleMonitor{
					Address:     mon.Address.Hex(),
					NRecords:    int(nRecords),
					FileSize:    file.FileSize(mon.Path()),
					LastScanned: mon.LastScanned,
					Deleted:     mon.Deleted,
				}
				if testMode {
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

func (opts *ListOptions) minMax() (ret base.FileRange) {
	ret.First = utils.Max(0, opts.FirstBlock)
	ret.Last = utils.Min(utils.NOPOS, opts.LastBlock)
	return
}
