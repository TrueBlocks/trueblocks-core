// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ExportOptions) HandleWithdrawals(monitorArray []monitor.Monitor) error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	first := utils.Max(base.KnownBlock(chain, "shanghai"), opts.FirstBlock)
	filter := filter.NewFilter(
		opts.Reversed,
		base.BlockRange{
			First: first,
			Last:  opts.LastBlock,
		},
		base.RecordRange{
			First: first,
			Last:  opts.GetMax(),
		},
	)
	filter.EnableRr = false

	// addrArray := make([]base.Address, 0, len(monitorArray))
	// for _, mon := range monitorArray {
	// 	addrArray = append(addrArray, mon.Address)
	// }

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawWithdrawal], errorChan chan error) {
		for _, mon := range monitorArray {
			mon := mon
			if items, err := opts.readWithdrawals(&mon, filter, errorChan); err != nil {
				errorChan <- err
				continue
			} else {
				for _, item := range items {
					item := item
					modelChan <- item
				}
			}
		}
	}

	extra := map[string]interface{}{
		"testMode": testMode,
		"export":   true,
	}

	if opts.Globals.Verbose || opts.Globals.Format == "json" {
		parts := names.Custom | names.Prefund | names.Regular
		if namesMap, err := names.LoadNamesMap(chain, parts, nil); err != nil {
			return err
		} else {
			extra["namesMap"] = namesMap
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
