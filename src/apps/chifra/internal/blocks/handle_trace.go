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

func (opts *BlocksOptions) HandleTrace() error {
	chain := opts.Globals.Chain

	// Don't do this in the loop
	meta, err := rpcClient.GetMetaData(chain, opts.Globals.TestMode)
	if err != nil {
		return err
	}
	if opts.Globals.TestMode {
		meta.Latest = 2000100
	}

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawTrace], errorChan chan error) {
		for _, br := range opts.BlockIds {
			blockNums, err := br.ResolveBlocks(chain)
			if err != nil {
				errorChan <- err
				if errors.Is(err, ethereum.NotFound) {
					continue
				}
				cancel()
				return
			}

			for _, bn := range blockNums {
				var traces []types.SimpleTrace
				traces, err = rpcClient.GetTracesByBlockNumber(chain, bn)
				if err != nil {
					errorChan <- err
					if errors.Is(err, ethereum.NotFound) {
						continue
					}
					cancel()
					return
				}

				for _, trace := range traces {
					// Note: This is needed because of a GoLang bug when taking the pointer of a loop variable
					trace := trace
					modelChan <- &trace
				}
			}
		}
	}

	extra := map[string]interface{}{
		"count":     opts.Count,
		"uncles":    opts.Uncles,
		"logs":      opts.Logs,
		"traces":    opts.Traces,
		"addresses": opts.Uniq,
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
