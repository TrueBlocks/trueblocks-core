// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tracesPkg

import (
	"context"
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *TracesOptions) HandleCounts() error {
	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		for _, ids := range opts.TransactionIds {
			txIds, err := ids.ResolveTxs(opts.Globals.Chain)
			if err != nil {
				errorChan <- err
				if errors.Is(err, ethereum.NotFound) {
					continue
				}
				cancel()
				return
			}

			for _, id := range txIds {
				tx, err := rpc.GetTxFromNumberAndId(opts.Globals.Chain, uint64(id.BlockNumber), uint64(id.TransactionIndex))
				if err != nil {
					errorChan <- err
					if errors.Is(err, ethereum.NotFound) {
						continue
					}
					cancel()
					return
				}

				txHash := tx.Hash().Hex()
				cnt, err := rpcClient.GetTracesCountByTransactionHash(opts.Globals.Chain, txHash)
				if err != nil {
					errorChan <- err
					if errors.Is(err, ethereum.NotFound) {
						continue
					}
					cancel()
					return
				}

				ts, err := tslib.FromBnToTs(opts.Globals.Chain, uint64(id.BlockNumber))
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
