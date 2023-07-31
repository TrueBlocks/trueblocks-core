// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package blocksPkg

import (
	"context"
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *BlocksOptions) HandleCounts() error {
	rpcOptions := rpcClient.DefaultRpcOptions(&rpcClient.DefaultRpcOptionsSettings{
		Chain: opts.Globals.Chain,
		Opts:  opts,
	})

	// TODO: Why does this have to dirty the caller?
	// If the cache is writeable, fetch the latest block timestamp so that we never
	// cache pending blocks
	if !rpcOptions.Store.ReadOnly() {
		rpcOptions.LatestBlockTimestamp = rpcClient.GetBlockTimestamp(opts.Globals.Chain, nil)
	}

	chain := opts.Globals.Chain

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
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
				var block types.SimpleBlock[string]
				if block, err = rpcClient.GetBlockHeaderByNumber(chain, bn, rpcClient.NoOptions); err != nil {
					errorChan <- err
					if errors.Is(err, ethereum.NotFound) {
						continue
					}
					cancel()
					return
				}

				blockCount := simpleBlockCount{
					BlockNumber:     block.BlockNumber,
					Timestamp:       block.Timestamp,
					TransactionsCnt: uint64(len(block.Transactions)),
				}

				if opts.Uncles {
					if blockCount.UnclesCnt, err = rpcClient.GetCountUnclesInBlock(chain, bn); err != nil {
						errorChan <- err
						if errors.Is(err, ethereum.NotFound) {
							continue
						}
						cancel()
						return
					}
				}

				if opts.Traces {
					if blockCount.TracesCnt, err = rpcClient.GetCountTracesInBlock(chain, bn); err != nil {
						errorChan <- err
						if errors.Is(err, ethereum.NotFound) {
							continue
						}
						cancel()
						return
					}
				}

				if opts.Logs {
					if blockCount.LogsCnt, err = rpcClient.GetLogCountByBlockNumber(chain, bn); err != nil {
						errorChan <- err
						if errors.Is(err, ethereum.NotFound) {
							continue
						}
						cancel()
						return
					}
				}

				if opts.Uniq {
					countFunc := func(s *types.SimpleAppearance) error {
						blockCount.AddressCnt++
						return nil
					}

					addrMap := make(index.AddressBooleanMap)
					ts := rpcClient.GetBlockTimestamp(chain, &bn)
					if err := opts.ProcessBlockUniqs(chain, countFunc, bn, addrMap, ts, rpcOptions); err != nil {
						errorChan <- err
						if errors.Is(err, ethereum.NotFound) {
							continue
						}
						cancel()
						return
					}
				}

				modelChan <- &blockCount
			}
		}
	}

	extra := map[string]interface{}{
		"count":  opts.Count,
		"uncles": opts.Uncles,
		"logs":   opts.Logs,
		"traces": opts.Traces,
		"uniqs":  opts.Uniq,
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
