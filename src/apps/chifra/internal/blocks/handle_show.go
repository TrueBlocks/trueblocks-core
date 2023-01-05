// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package blocksPkg

import (
	"context"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *BlocksOptions) HandleShowBlocks() error {
	// Don't do this in the loop
	meta, err := rpcClient.GetMetaData(opts.Globals.Chain, opts.Globals.TestMode)
	if err != nil {
		return err
	}

	ctx, cancel := context.WithCancel(context.Background())

	// Note: Make sure to add an entry to enabledForCmd in src/apps/chifra/pkg/output/helpers.go
	fetchData := func(modelChan chan types.Modeler[types.RawBlock], errorChan chan error) {
		for _, br := range opts.BlockIds {
			blockNums, err := br.ResolveBlocks(opts.Globals.Chain)
			if err != nil {
				errorChan <- err
				cancel()
				return
			}
			for _, bn := range blockNums {
				finalized := meta.Age(bn) > 28

				// Decide on the concrete type of block.Transactions and set values
				var block types.Modeler[types.RawBlock]
				var err error
				if !opts.Hashes {
					var b types.SimpleBlock[types.SimpleTransaction]
					b, err = rpcClient.GetBlockByNumberWithTxs(opts.Globals.Chain, bn, finalized)
					block = &b
				} else {
					var b types.SimpleBlock[string]
					b, err = rpcClient.GetBlockByNumber(opts.Globals.Chain, bn, finalized)
					block = &b
				}

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
				modelChan <- block
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
		Extra: map[string]interface{}{
			"txHashes": opts.Hashes,
		},
	})
}
