// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
	tslibPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *WhenOptions) HandleWhenShowTimestamps() error {
	cnt, err := tslibPkg.NTimestamps(opts.Globals.Chain)
	if err != nil {
		return err
	}

	if !opts.Check {
		err = opts.Globals.RenderHeader(tslibPkg.Timestamp{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
		defer opts.Globals.RenderFooter(opts.Globals.ApiMode || opts.Globals.Format == "api")
		if err != nil {
			return err
		}
		for bn := uint64(0); bn < cnt; bn++ {
			obj, err := tslibPkg.FromBn(opts.Globals.Chain, bn)
			if err != nil {
				return err
			}
			err = opts.Globals.RenderObject(*obj, bn == 0)
			if err != nil {
				return err
			}
		}
		return nil
	}

	scanBar := progress.NewScanBar(cnt /* wanted */, cnt/500 /* freq */, cnt /* max */, (2. / 3.))
	for bn := uint64(0); bn < cnt; bn++ {
		item, err := tslibPkg.FromBn(opts.Globals.Chain, bn)
		if err != nil {
			return err
		}
		block := types.NamedBlock{} //rpcClient.GetBlockByNumber(opts.Globals.Chain, bn)
		if bn == 0 {
			block.TimeStamp = utils.EarliestTs
		}
		msg := fmt.Sprintf("%d-%d-%d-%d-%d", block.BlockNumber, block.TimeStamp, bn, item.Bn, item.Ts)
		scanBar.Report(opts.Globals.Writer, "Checking", msg)
	}
	return nil
}
