// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// HandleTimestampsShow handles chifra when --timestamps
func (opts *WhenOptions) HandleTimestampsShow() error {
	bnMap, err := identifiers.GetBlockNumberMap(opts.Globals.Chain, opts.BlockIds)
	if err != nil {
		return err
	}

	var cnt uint64
	cnt, err = tslib.NTimestamps(opts.Globals.Chain)
	if err != nil {
		return err
	}

	ctx := context.Background()
	prev := base.Timestamp(0)
	fetchData := func(modelChan chan types.Modeler[types.RawTimestamp], errorChan chan error) {
		for bn := uint64(0); bn < cnt; bn++ {
			if len(bnMap) == 0 || bnMap[bn] {
				ts, err := tslib.FromBn(opts.Globals.Chain, bn)
				if err != nil {
					errorChan <- err
				}
				s := types.SimpleTimestamp{
					BlockNumber: uint64(ts.Bn),
					Timestamp:   base.Timestamp(ts.Ts),
					Diff:        base.Timestamp(ts.Ts) - prev,
				}
				if bn == 0 {
					s.Diff = 0
				}
				modelChan <- &s
				prev = s.Timestamp
			}
		}
	}

	return output.StreamMany(ctx, fetchData, output.OutputOptions{
		Writer:     opts.Globals.Writer,
		Chain:      opts.Globals.Chain,
		TestMode:   opts.Globals.TestMode,
		NoHeader:   opts.Globals.NoHeader,
		ShowRaw:    opts.Globals.ShowRaw,
		Verbose:    opts.Globals.Verbose,
		LogLevel:   opts.Globals.LogLevel,
		Format:     opts.Globals.Format,
		OutputFn:   opts.Globals.OutputFn,
		Append:     opts.Globals.Append,
		JsonIndent: "  ",
	})
}
