// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package listPkg

import (
	"context"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ListOptions) HandleShow(rCtx *output.RenderCtx, monitorArray []monitor.Monitor) error {
	if opts.Silent {
		return nil
	}

	chain := opts.Globals.Chain
	filter := filter.NewFilter(
		opts.Reversed,
		false,
		[]string{},
		base.BlockRange{First: opts.FirstBlock, Last: opts.LastBlock},
		base.RecordRange{First: opts.FirstRecord, Last: opts.GetMax()},
	)

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		currentBn := uint32(0)
		currentTs := base.Timestamp(0)
		visitAppearance := func(app *types.Appearance) error {
			if opts.Globals.Verbose {
				if app.BlockNumber == 0 || app.BlockNumber != currentBn {
					currentTs, _ = tslib.FromBnToTs(chain, base.Blknum(app.BlockNumber))
				}
				app.Timestamp = currentTs
				currentBn = app.BlockNumber
			}
			modelChan <- app
			return nil
		}

		for _, mon := range monitorArray {
			if apps, cnt, err := mon.ReadAndFilterAppearances(filter, true /* withCount */); err != nil {
				errorChan <- err
				continue // on error
			} else if !opts.NoZero || cnt > 0 {
				for _, app := range apps {
					if err := visitAppearance(&app); err != nil {
						errorChan <- err
						return
					}
				}
			} else {
				errorChan <- fmt.Errorf("no appearances found for %s", mon.Address.Hex())
				continue
			}
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}

func (opts *ListOptions) IsMax(cnt uint64) bool {
	max := opts.MaxRecords
	if max == 250 && !opts.Globals.IsApiMode() {
		max = base.NOPOS
	}
	return cnt >= max
}

func (opts *ListOptions) GetMax() uint64 {
	if opts.MaxRecords == 250 && !opts.Globals.IsApiMode() {
		return base.NOPOS
	}
	return opts.MaxRecords
}

// TODO: We used to keep these stats on the chifra list process. Lost during port from C++ currently stored in $HOME/.../TrueBlocks/perf
// We could use Graphenpa or something to display them.
// n BloomHits
// n BloomMisses
// n CacheWrites
// n Checked
// n FalsePositive
// n FileRecords
// n Files
// n Positive
// n Skipped
// n StageChecked
// n StageHits
// n TotalHits
