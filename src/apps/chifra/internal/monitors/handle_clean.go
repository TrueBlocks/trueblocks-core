// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitorsPkg

import (
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// HandleClean handles the chifra monitors --clean command.
func (opts *MonitorsOptions) HandleClean(rCtx *output.RenderCtx) error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		_, monArray := GetMonitorMap(chain)
		for _, mon := range monArray {
			addr := mon.Address.Hex()
			s := types.MonitorClean{
				Address: mon.Address,
				Staged:  mon.Staged,
			}
			if testMode {
				if addr == "0x001d14804b399c6ef80e64576f657660804fec0b" ||
					addr == "0x0029218e1dab069656bfb8a75947825e7989b987" {
					s.SizeThen = 10
					s.SizeNow = 8
					s.Dups = 10 - 8
				}
			} else {
				then, now, err := mon.RemoveDups()
				if err != nil {
					errorChan <- err
					return
				}
				s.SizeThen = int64(then)
				s.SizeNow = int64(now)
				s.Dups = s.SizeThen - s.SizeNow
				if opts.Staged && mon.Staged {
					s.Removed = true
					mon.Close()
					_ = mon.Delete()
					_, _ = mon.Remove()
				}
			}

			if s.SizeThen > 0 {
				modelChan <- &s
			}
		}
	}

	extraOpts := map[string]any{
		"staged": opts.Staged,
	}
	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}

func GetMonitorMap(chain string) (map[base.Address]*monitor.Monitor, []*monitor.Monitor) {
	monitorChan := make(chan monitor.Monitor)

	go monitor.ListExistingMonitors(chain, monitorChan)

	monMap := make(map[base.Address]*monitor.Monitor)
	monArray := []*monitor.Monitor{}
	for mon := range monitorChan {
		switch mon.Address {
		case base.NotAMonitor:
			close(monitorChan)
		default:
			monMap[mon.Address] = &mon
			monArray = append(monArray, &mon)
		}
	}

	sort.Slice(monArray, func(i, j int) bool {
		return monArray[i].Address.Hex() < monArray[j].Address.Hex()
	})

	return monMap, monArray
}
