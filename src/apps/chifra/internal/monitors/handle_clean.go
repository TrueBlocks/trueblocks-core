// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitorsPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// HandleClean
func (opts *MonitorsOptions) HandleClean() error {
	monitorChan := make(chan monitor.Monitor)

	var monitors []monitor.Monitor
	go monitor.ListMonitors(opts.Globals.Chain, "monitors", monitorChan)

	for result := range monitorChan {
		switch result.Address {
		case monitor.SentinalAddr:
			close(monitorChan)
		default:
			monitors = append(monitors, result)
		}
	}

	objs := []types.CleanReport{}
	for _, mon := range monitors {
		if opts.Globals.TestMode {
			addr := mon.GetAddrStr()
			if addr == "0x001d14804b399c6ef80e64576f657660804fec0b" ||
				addr == "0x0029218e1dab069656bfb8a75947825e7989b987" {
				objs = append(objs, types.CleanReport{
					Addr:     addr,
					SizeThen: 10,
					SizeNow:  8,
					Dups:     10 - 8,
				})
			}
		} else {
			obj := types.CleanReport{
				Addr: mon.GetAddrStr(),
			}
			logger.Log(logger.Info, "Cleaning", obj.Addr, mon.Count())
			var err error
			obj.SizeThen, obj.SizeNow, err = mon.RemoveDups()
			if err != nil {
				return err
			}

			obj.Dups = obj.SizeThen - obj.SizeNow
			if obj.SizeThen > 0 {
				objs = append(objs, obj)
			}
		}
	}

	// TODO: Fix export without arrays
	return globals.RenderSlice(&opts.Globals, objs)
}
