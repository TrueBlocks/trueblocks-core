// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
)

// HandleTimestampsShow handles chifra when --timestamps
func (opts *WhenOptions) HandleTimestampsShow(rCtx *output.RenderCtx) error {
	chain := opts.Globals.Chain

	bnMap, err := identifiers.GetBlockNumberMap(chain, opts.BlockIds)
	if err != nil {
		return err
	}

	cnt, err := tslib.NTimestamps(chain)
	if err != nil {
		return err
	}

	prev := base.Timestamp(0)
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		for bn := base.Blknum(0); bn < cnt; bn++ {
			if len(bnMap) == 0 || bnMap[bn] {
				ts, err := tslib.FromBn(chain, bn)
				if err != nil {
					errorChan <- err
				}
				s := types.Timestamp{
					BlockNumber: base.Blknum(ts.Bn),
					Timestamp:   base.Timestamp(ts.Ts),
					Diff:        int64(base.Timestamp(ts.Ts) - prev),
				}
				if bn == 0 {
					s.Diff = 0
				}
				modelChan <- &s
				prev = s.Timestamp
			}
		}
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}
