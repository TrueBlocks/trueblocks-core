// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *WhenOptions) HandleWhenShowTimestamps() error {
	cnt, err := tslib.NTimestamps(opts.Globals.Chain)
	if err != nil {
		return err
	}

	if !opts.Check {
		err = opts.Globals.RenderHeader(tslib.Timestamp{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
		defer opts.Globals.RenderFooter()
		if err != nil {
			return err
		}
		prev := types.SimpleTimestamp{}
		for bn := uint64(0); bn < cnt; bn++ {
			ts, err := tslib.FromBn(opts.Globals.Chain, bn)
			if err != nil {
				return err
			}
			obj := types.SimpleTimestamp{
				BlockNumber: uint64(ts.Bn),
				TimeStamp:   uint64(ts.Ts),
				Diff:        uint64(ts.Ts - uint32(prev.TimeStamp)),
			}
			if ts.Bn == 0 {
				obj.Diff = 0
			}
			err = opts.Globals.RenderObject(obj, bn == 0)
			if err != nil {
				return err
			}
			prev = obj
		}
		return nil
	}

	scanBar := progress.NewScanBar(cnt /* wanted */, cnt/500 /* freq */, cnt /* max */, (2. / 3.))
	for bn := uint64(0); bn < cnt; bn++ {
		item, err := tslib.FromBn(opts.Globals.Chain, bn)
		if err != nil {
			return err
		}
		block := types.SimpleNamedBlock{} //rpcClient.GetBlockByNumber(opts.Globals.Chain, bn)
		if bn == 0 {
			block.TimeStamp, _ = rpcClient.GetBlockZeroTs(opts.Globals.Chain)
		}
		msg := fmt.Sprintf("%d-%d-%d-%d-%d", block.BlockNumber, block.TimeStamp, bn, item.Bn, item.Ts)
		scanBar.Report(opts.Globals.Writer, "Checking", msg)
	}
	return nil
}
