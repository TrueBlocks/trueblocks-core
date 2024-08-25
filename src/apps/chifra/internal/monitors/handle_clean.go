// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitorsPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// HandleClean handles the chifra monitors --clean command.
func (opts *MonitorsOptions) HandleClean(rCtx *output.RenderCtx) error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	_, monArray := monitor.GetMonitorMap(chain)

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		for _, mon := range monArray {
			addr := mon.Address.Hex()
			s := types.MonitorClean{
				Address: mon.Address,
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
			}

			if s.SizeThen > 0 {
				modelChan <- &s
			}
			// delete empty monitors
		}
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}
