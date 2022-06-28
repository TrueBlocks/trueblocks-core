// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	tslibPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *WhenOptions) HandleWhenShowBlocks() error {
	// TODO: Fix export without arrays
	err := opts.Globals.RenderHeader(types.NamedBlock{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
	defer opts.Globals.RenderFooter()
	if err != nil {
		return err
	}

	for i, br := range opts.BlockIds {
		blockNums, err := br.ResolveBlocks(opts.Globals.Chain)
		if err != nil {
			return err
		}
		for _, bn := range blockNums {
			block, err := rpcClient.GetBlockByNumber(opts.Globals.Chain, bn)
			if err != nil {
				return err
			}
			d, _ := tslibPkg.FromTsToDate(block.TimeStamp)
			nm, _ := tslibPkg.FromBnToName(opts.Globals.Chain, block.BlockNumber)
			block.Date = d.Format("YYYY-MM-DD HH:mm:ss UTC")
			block.Name = nm
			err = opts.Globals.RenderObject(block, i == 0)
			if err != nil {
				return err
			}
		}
	}

	return nil
}
