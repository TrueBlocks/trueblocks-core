// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tracesPkg

import (
	"context"
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
	"github.com/ethereum/go-ethereum/common"
)

func (opts *TracesOptions) HandleCounts() error {
	ctx, cancel := context.WithCancel(context.Background())

	// Note: Make sure to add an entry to enabledForCmd in src/apps/chifra/pkg/output/helpers.go
	fetchData := func(modelChan chan types.Modeler[types.RawTraceCount], errorChan chan error) {
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
				tx, err := rpc.TxFromNumberAndId(opts.Globals.Chain, uint64(id.BlockNumber), uint64(id.TransactionIndex))
				if err != nil {
					errorChan <- err
					if errors.Is(err, ethereum.NotFound) {
						continue
					}
					cancel()
					return
				}

				txHash := tx.Hash().Hex()
				cnt, err := types.GetTracesCountByTransactionHash(opts.Globals.Chain, txHash)
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

				counter := types.SimpleTraceCount{
					BlockNumber:      uint64(id.BlockNumber),
					TransactionIndex: uint64(id.TransactionIndex),
					TransactionHash:  common.HexToHash(txHash),
					Timestamp:        int64(ts),
					TracesCnt:        cnt,
				}
				modelChan <- &counter
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
