// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"context"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *WhenOptions) HandleShowBlocks() error {

	ctx, cancel := context.WithCancel(context.Background())

	// Note: Make sure to add an entry to enabledForCmd in src/apps/chifra/pkg/output/helpers.go
	fetchData := func(modelChan chan types.Modeler[types.RawNamedBlock], errorChan chan error) {
		for _, br := range opts.BlockIds {
			blockNums, err := br.ResolveBlocks(opts.Globals.Chain)
			if err != nil {
				errorChan <- err
				cancel()
				return
			}
			for _, bn := range blockNums {
				block, err := rpcClient.GetBlockHeaderByNumber(opts.Globals.Chain, bn)
				// TODO: rpcClient should return a custom type of error in this case
				if err != nil && strings.Contains(err.Error(), "not found") {
					errorChan <- err
					continue
				}
				if err != nil {
					errorChan <- err
					cancel()
					return
				}
				d, _ := tslib.FromTsToDate(block.GetTimestamp())
				nm, _ := tslib.FromBnToName(opts.Globals.Chain, block.BlockNumber)
				modelChan <- &types.SimpleNamedBlock{
					BlockNumber: block.BlockNumber,
					Timestamp:   block.GetTimestamp(),
					Date:        d.Format("YYYY-MM-DD HH:mm:ss UTC"),
					Name:        nm,
				}
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
