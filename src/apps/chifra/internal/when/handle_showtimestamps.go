// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// HandleWhenShowTimestamps handles chifra when --timestamps
func (opts *WhenOptions) HandleWhenShowTimestamps() error {
	cnt, err := tslib.NTimestamps(opts.Globals.Chain)
	if err != nil {
		return err
	}

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
