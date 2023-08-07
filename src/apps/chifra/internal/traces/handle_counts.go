// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tracesPkg

import (
	"context"
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *TracesOptions) HandleCounts() error {
	chain := opts.Globals.Chain

	// TODO: Why does this have to dirty the caller?
	settings := rpcClient.Settings{
		Chain: chain,
		Opts:  opts,
	}
	opts.Conn = settings.GetRpcConnection()

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		for _, ids := range opts.TransactionIds {
			txIds, err := ids.ResolveTxs(chain)
			if err != nil {
				errorChan <- err
				if errors.Is(err, ethereum.NotFound) {
					continue
				}
				cancel()
				return
			}

			for _, id := range txIds {
				txHash, err := opts.Conn.GetEtherumTxHash(uint64(id.BlockNumber), uint64(id.TransactionIndex))
				if err != nil {
					errorChan <- err
					if errors.Is(err, ethereum.NotFound) {
						continue
					}
					cancel()
					return
				}

				cnt, err := opts.Conn.GetTracesCountInTransaction(txHash)
				if err != nil {
					errorChan <- err
					if errors.Is(err, ethereum.NotFound) {
						continue
					}
					cancel()
					return
				}

				ts, err := tslib.FromBnToTs(chain, uint64(id.BlockNumber))
				if err != nil {
					errorChan <- err
					if errors.Is(err, ethereum.NotFound) {
						continue
					}
					cancel()
					return
				}

				counter := simpleTraceCount{
					BlockNumber:      uint64(id.BlockNumber),
					TransactionIndex: uint64(id.TransactionIndex),
					TransactionHash:  base.HexToHash(txHash),
					Timestamp:        ts,
					TracesCnt:        cnt,
				}
				modelChan <- &counter
			}
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
