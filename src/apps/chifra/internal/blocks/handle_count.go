// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package blocksPkg

import (
	"context"
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/uniq"
	"github.com/ethereum/go-ethereum"
)

func (opts *BlocksOptions) HandleCount() error {
	chain := opts.Globals.Chain

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
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
				var block types.LightBlock
				if block, err = opts.Conn.GetBlockHeaderByNumber(bn); err != nil {
					errorChan <- err
					if errors.Is(err, ethereum.NotFound) {
						continue
					}
					cancel()
					return
				}

				blockCount := types.BlockCount{
					BlockNumber:     block.BlockNumber,
					Timestamp:       block.Timestamp,
					TransactionsCnt: uint64(len(block.Transactions)),
					WithdrawalsCnt:  uint64(len(block.Withdrawals)),
				}

				if opts.Uncles {
					if blockCount.UnclesCnt, err = opts.Conn.GetUnclesCountInBlock(bn); err != nil {
						errorChan <- err
						if errors.Is(err, ethereum.NotFound) {
							continue
						}
						cancel()
						return
					}
				}

				if opts.Traces {
					if blockCount.TracesCnt, err = opts.Conn.GetTracesCountInBlock(bn); err != nil {
						errorChan <- err
						if errors.Is(err, ethereum.NotFound) {
							continue
						}
						cancel()
						return
					}
				}

				if opts.Logs {
					if blockCount.LogsCnt, err = opts.Conn.GetLogsCountInBlock(bn, block.Timestamp); err != nil {
						errorChan <- err
						if errors.Is(err, ethereum.NotFound) {
							continue
						}
						cancel()
						return
					}
				}

				if opts.Uniq {
					countFunc := func(s *types.Appearance) error {
						blockCount.AddressCnt++
						return nil
					}

					if err := uniq.GetUniqAddressesInBlock(chain, opts.Flow, opts.Conn, countFunc, bn); err != nil {
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

	extraOpts := map[string]any{
		"count":  opts.Count,
		"uncles": opts.Uncles,
		"logs":   opts.Logs,
		"traces": opts.Traces,
		"uniqs":  opts.Uniq,
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}
