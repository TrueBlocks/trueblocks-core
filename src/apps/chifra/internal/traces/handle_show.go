// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tracesPkg

import (
	"context"
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *TracesOptions) HandleShowTraces() error {
	ctx, cancel := context.WithCancel(context.Background())

	// Note: Make sure to add an entry to enabledForCmd in src/apps/chifra/pkg/output/helpers.go
	fetchData := func(modelChan chan types.Modeler[types.RawTrace], errorChan chan error) {
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

			// Timestamp is not part of the raw trace data so we need to get it separately
			// TxIds don't span blocks, so we can use the first one outside the loop to find timestamp
			ts := rpc.GetBlockTimestamp(opts.Globals.Chain, uint64(txIds[0].BlockNumber))
			for _, id := range txIds {
				// Decide on the concrete type of block.Transactions and set values
				traces, err := types.GetTracesByTransactionId(opts.Globals.Chain, uint64(id.BlockNumber), uint64(id.TransactionIndex))
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
					trace.Timestamp = ts
					modelChan <- &trace
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
