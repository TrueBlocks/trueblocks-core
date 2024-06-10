// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitorsPkg

import (
	"context"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// HandleList handles the chifra monitors --list command.
func (opts *MonitorsOptions) HandleList() error {
	chain := opts.Globals.Chain
	monitorMap, monArray := monitor.GetMonitorMap(chain)
	if opts.Globals.Verbose {
		for i := 0; i < len(monArray); i++ {
			_ = monArray[i].ReadMonitorHeader()
			monArray[i].Close()
		}
	}

	errors := make([]error, 0)
	addrMap := map[base.Address]bool{}
	for _, addr := range opts.Addrs {
		a := base.HexToAddress(addr)
		addrMap[a] = true
		if monitorMap[a] == nil {
			errors = append(errors, fmt.Errorf("address %s is not being monitored", addr))
		}
	}

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		for _, e := range errors {
			errorChan <- e
		}

		for _, mon := range monArray {
			if len(addrMap) == 0 || addrMap[mon.Address] {
				s := types.Monitor{
					Address:     mon.Address,
					NRecords:    mon.Count(),
					FileSize:    file.FileSize(mon.Path()),
					LastScanned: mon.LastScanned,
					Deleted:     mon.Deleted,
				}
				modelChan <- &s
			}
		}
	}

	extraOpts := map[string]any{}
	if opts.Globals.Verbose && opts.Globals.Format == "json" {
		parts := names.Custom | names.Prefund | names.Regular
		namesMap, err := names.LoadNamesMap(chain, parts, nil)
		if err != nil {
			return err
		}
		extraOpts["namesMap"] = namesMap
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}
