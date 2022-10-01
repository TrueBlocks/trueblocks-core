// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// HandleTimestampsShow handles chifra when --timestamps
func (opts *WhenOptions) HandleTimestampsShow() error {
	cnt, err := tslib.NTimestamps(opts.Globals.Chain)
	if err != nil {
		return err
	}

	err = opts.Globals.RenderHeader(types.SimpleTimestamp{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.NoHeader, true)
	defer opts.Globals.RenderFooter()
	if err != nil {
		return err
	}

	prev := types.SimpleTimestamp{}
	blockNums, err := identifiers.GetBlockNumbers(opts.Globals.Chain, opts.BlockIds)
	if err != nil {
		return err
	}

	if len(blockNums) > 0 {
		for i, bn := range blockNums {
			if err = opts.showOneBlock(&prev, bn, i == 0); err != nil {
				return err
			}
		}
	} else {
		for bn := uint64(0); bn < cnt; bn++ {
			if err = opts.showOneBlock(&prev, bn, bn == 0); err != nil {
				return err
			}
		}
	}

	return nil
}

func (opts *WhenOptions) showOneBlock(prev *types.SimpleTimestamp, bn uint64, first bool) error {
	ts, err := tslib.FromBn(opts.Globals.Chain, bn)
	if err != nil {
		return err
	}
	obj := types.SimpleTimestamp{
		BlockNumber: uint64(ts.Bn),
		TimeStamp:   uint64(ts.Ts),
		Diff:        uint64(ts.Ts - uint32(prev.TimeStamp)),
	}
	if first {
		// Report zero diff at first block
		obj.Diff = 0
	}
	err = opts.Globals.RenderObject(obj, bn == 0)
	if err != nil {
		return err
	}
	*prev = obj

	return nil
}
