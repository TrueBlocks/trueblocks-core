// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *WhenOptions) HandleTimestampCount() error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		if count, err := tslib.NTimestamps(chain); err != nil {
			errorChan <- err
			return

		} else {
			if testMode {
				count = 5000000
			}

			s := simpleWhenCount{
				Count: count,
			}

			modelChan <- &s
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}

// TODO: BOGUS - MUST DOCUMENT
type simpleWhenCount struct {
	Count uint64 `json:"count"`
}

func (s *simpleWhenCount) Raw() *types.RawModeler {
	return nil
}

func (s *simpleWhenCount) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
	return types.Model{
		Data: map[string]interface{}{
			"count": s.Count,
		},
		Order: []string{
			"count",
		},
	}
}
