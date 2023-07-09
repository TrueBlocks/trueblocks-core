// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"context"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ExportOptions) HandleAppearances(monitorArray []monitor.Monitor) error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	max := opts.MaxRecords
	if max == 250 && !opts.Globals.IsApiMode() {
		max = utils.NOPOS
	}
	filter := &AppearanceFilter{
		logging:     !testMode,
		reversed:    opts.Reversed,
		firstRecord: opts.FirstRecord,
		maxRecords:  max,
		exportRange: base.FileRange{First: opts.FirstBlock, Last: opts.LastBlock},
		nSeen:       int64(-1),
		nExported:   0,
	}

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawAppearance], errorChan chan error) {
		visitAppearance := func(app *types.SimpleAppearance) error {
			modelChan <- app
			return nil
		}

		for _, mon := range monitorArray {
			if apps, cnt, err := ReadAppearancesToSlice2(&mon, filter); err != nil {
				errorChan <- err
				return
			} else if cnt == 0 {
				errorChan <- fmt.Errorf("no appearances found for %s", mon.Address.Hex())
				return
			} else {
				for _, app := range apps {
					app := app
					if opts.Globals.Verbose {
						app.Timestamp = filter.GetTimestamp(chain, app.BlockNumber)
					}
					if err := visitAppearance(&app); err != nil {
						errorChan <- err
						return
					}
				}
			}
		}
	}

	extra := map[string]interface{}{
		"articulate": opts.Articulate,
		"testMode":   testMode,
		"export":     true,
	}

	if opts.Globals.Verbose || opts.Globals.Format == "json" {
		parts := names.Custom | names.Prefund | names.Regular
		namesMap, err := names.LoadNamesMap(chain, parts, nil)
		if err != nil {
			return err
		}
		extra["namesMap"] = namesMap
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}

func (opts *ExportOptions) IsMax(cnt uint64) bool {
	max := opts.MaxRecords
	if max == 250 && !opts.Globals.IsApiMode() {
		max = utils.NOPOS
	}
	return cnt >= max
}
