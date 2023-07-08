// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"context"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ExportOptions) HandleCount(monitorArray []monitor.Monitor) error {
	testMode := opts.Globals.TestMode
	sortBy := monitor.Sorted
	if opts.Reversed {
		sortBy = monitor.Reversed
	}
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
			if true { // !opts.NoZero || mon.Count() > 0 {
				if apps, cnt, err := mon.ReadAppearancesToSlice(sortBy); err != nil {
					errorChan <- err
					return
				} else if cnt == 0 {
					errorChan <- fmt.Errorf("no appearances found for %s", mon.Address.Hex())
					return
				} else {
					nRecords := 0
					for _, app := range apps {
						if exportRange.IntersectsB(uint64(app.BlockNumber)) {
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
						s.FileSize = 0xdead
						s.LastScanned = maxTestingBlock
					}
					modelChan <- &s
				}
			}
			mon.Close()
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}

func (opts *ExportOptions) minMax() (ret base.FileRange) {
	ret.First = utils.Max(0, opts.FirstBlock)
	ret.Last = utils.Min(utils.NOPOS, opts.LastBlock)
	return
}

const maxTestingBlock = 17000000
