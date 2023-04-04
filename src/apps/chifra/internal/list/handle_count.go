// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package listPkg

import (
	"context"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type countMode = int

const (
	modeNone countMode = iota
	modeRange
	modeFirstBlock
	modeLastBlock
)

func getCountMode(opts *ListOptions) countMode {
	if opts.FirstBlock != utils.NOPOS && opts.LastBlock != utils.NOPOS {
		return modeRange
	}

	if opts.FirstBlock != utils.NOPOS {
		return modeFirstBlock
	}

	if opts.LastBlock != utils.NOPOS {
		return modeLastBlock
	}

	return modeNone
}

func (opts *ListOptions) HandleListCount(monitorArray []monitor.Monitor) error {
	testMode := opts.Globals.TestMode
	exportRange := base.FileRange{First: opts.FirstBlock, Last: opts.LastBlock}

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawMonitor], errorChan chan error) {
		for _, mon := range monitorArray {
			fmt.Println(">>> c", mon.Count())
			if !opts.NoZero || mon.Count() > 0 {

				mode := getCountMode(opts)
				fmt.Println(">>> mode", mode, opts.FirstBlock, opts.LastBlock)
				records := mon.Count()
				if mode != modeNone {
					records = 0
					apps := make([]index.AppearanceRecord, 0, mon.Count())
					if err := mon.ReadAppearances(&apps); err != nil {
						errorChan <- err
						continue
					}
					for _, appearance := range apps {
						switch mode {
						case modeRange:
							appRange := base.FileRange{
								First: uint64(appearance.BlockNumber),
								Last:  uint64(appearance.BlockNumber),
							}
							if appRange.Intersects(exportRange) {
								records++
							}
						case modeFirstBlock:
							fmt.Println(">>>", "app", appearance.BlockNumber, "first block", uint32(opts.FirstBlock))
							if appearance.BlockNumber >= uint32(opts.FirstBlock) {
								records++
							}
						case modeLastBlock:
							if appearance.BlockNumber <= uint32(opts.LastBlock) {
								records++
							}
						}
					}
				}

				s := types.SimpleMonitor{
					Address:     mon.Address.Hex(),
					NRecords:    int(records),
					FileSize:    file.FileSize(mon.Path()),
					LastScanned: mon.Header.LastScanned,
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
