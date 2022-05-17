// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	tslibPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *WhenOptions) HandleWhenBlocks() error {
	// TODO: Fix export without arrays
	err := opts.Globals.RenderHeader(types.NamedBlock{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
	defer opts.Globals.RenderFooter(opts.Globals.ApiMode || opts.Globals.Format == "api")
	if err != nil {
		return err
	}

	first := true
	for _, br := range opts.BlockNums {
		vals, err := br.Resolve(opts.Globals.Chain)
		if err != nil {
			return err
		}
		for _, v := range vals {
			block := rpcClient.GetBlockByNumber(opts.Globals.Chain, v)
			if v == 0 {
				block.TimeStamp = utils.EarliestTs
			}
			d, _ := tslibPkg.FromTsToDate(block.TimeStamp)
			nm, _ := tslibPkg.FromBnToName(opts.Globals.Chain, block.BlockNumber)
			block.Date = d.Format("YYYY-MM-DD HH:mm:ss UTC")
			block.Name = nm
			err := opts.Globals.RenderObject(block, false, first)
			first = false
			if err != nil {
				return err
			}
		}
	}

	return nil
}
