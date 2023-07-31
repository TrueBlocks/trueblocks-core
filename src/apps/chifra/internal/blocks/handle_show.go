// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package blocksPkg

import (
	"context"
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *BlocksOptions) HandleShowBlocks() error {
	rpcOptions := rpcClient.DefaultRpcOptions(&rpcClient.DefaultRpcOptionsSettings{
		Chain: opts.Globals.Chain,
		Opts:  opts,
	})

	// TODO: Why does this have to dirty the caller?
	// If the cache is writeable, fetch the latest block timestamp so that we never
	// cache pending blocks
	if !rpcOptions.Store.ReadOnly() {
		rpcOptions.LatestBlockTimestamp = rpc.GetBlockTimestamp(opts.Globals.Chain, nil)
	}

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawBlock], errorChan chan error) {
		for _, br := range opts.BlockIds {
			blockNums, err := br.ResolveBlocks(opts.Globals.Chain)
			if err != nil {
				errorChan <- err
				if errors.Is(err, ethereum.NotFound) {
					continue
				}
				cancel()
				return
			}

			for _, bn := range blockNums {
				// Decide on the concrete type of block.Transactions and set values
				var block types.Modeler[types.RawBlock]
				var err error
				if !opts.Hashes {
					var b types.SimpleBlock[types.SimpleTransaction]
					b, err = rpcClient.GetBlockBodyByNumber(opts.Globals.Chain, bn, rpcOptions)
					block = &b
				} else {
					var b types.SimpleBlock[string]
					b, err = rpcClient.GetBlockHeaderByNumber(opts.Globals.Chain, bn, rpcOptions)
					block = &b
				}

				if err != nil {
					errorChan <- err
					if errors.Is(err, ethereum.NotFound) {
						continue
					}
					cancel()
					return
				}

				modelChan <- block
			}
		}
	}

	extra := map[string]interface{}{
		"hashes":     opts.Hashes,
		"count":      opts.Count,
		"uncles":     opts.Uncles,
		"articulate": opts.Articulate,
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
