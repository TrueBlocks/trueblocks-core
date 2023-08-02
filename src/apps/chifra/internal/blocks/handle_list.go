// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package blocksPkg

import (
	"context"
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *BlocksOptions) HandleList() error {
	chain := opts.Globals.Chain

	// Don't do this in the loop
	meta, err := opts.Conn.GetMetaData(chain, opts.Globals.TestMode)
	if err != nil {
		return err
	}
	if opts.Globals.TestMode {
		meta.Latest = 2000100
	}
	start := meta.Latest - opts.List
	end := start - opts.ListCount
	if start < opts.ListCount {
		end = 0
	}

	// TODO: Why does this have to dirty the caller?
	settings := rpcClient.DefaultRpcOptionsSettings{
		Chain: chain,
		Opts:  opts,
	}
	opts.Conn = settings.DefaultRpcOptions()
	if !opts.Conn.Store.ReadOnly() {
		opts.Conn.LatestBlockTimestamp = opts.Conn.GetBlockTimestamp(chain, nil)
	}

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawBlock], errorChan chan error) {
		for bn := start; bn > end; bn-- {
			block, err := opts.Conn.GetBlockHeaderByNumber(chain, bn)
			if err != nil {
				errorChan <- err
				if errors.Is(err, ethereum.NotFound) {
					continue
				}
				cancel()
				return
			}

			modelChan <- &block
		}
	}

	extra := map[string]interface{}{
		"list": true,
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
