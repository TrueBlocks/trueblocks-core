// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *WhenOptions) HandleTimestampsCount(rCtx *output.RenderCtx) error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		if count, err := tslib.NTimestamps(chain); err != nil {
			errorChan <- err
			return

		} else {
			if testMode {
				count = 5000000
			}

			s := types.Count{
				Count: uint64(count),
			}

			modelChan <- &s
		}
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}
