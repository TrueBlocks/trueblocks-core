// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tracesPkg

import (
	"context"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *TracesOptions) HandleCounts() error {
	ctx, cancel := context.WithCancel(context.Background())

	// Note: Make sure to add an entry to enabledForCmd in src/apps/chifra/pkg/output/helpers.go
	fetchData := func(modelChan chan types.Modeler[types.RawTraceCount], errorChan chan error) {
		for _, ids := range opts.TransactionIds {
			txIds, err := ids.ResolveTxs(opts.Globals.Chain)
			if err != nil {
				errorChan <- err
				cancel()
				return
			}
			for _, id := range txIds {
				// Decide on the concrete type of block.Transactions and set values
				cnt, err := types.GetTracesCountByTransactionId(opts.Globals.Chain, uint64(id.BlockNumber), uint64(id.TransactionIndex))
				if err != nil && strings.Contains(err.Error(), "not found") {
					errorChan <- err
					continue
				}
				if err != nil {
					errorChan <- err
					cancel()
					return
				}
				counter := types.SimpleTraceCount{
					BlockNumber:      uint64(id.BlockNumber),
					TransactionIndex: uint64(id.TransactionIndex),
					// TransactionHash: "0x0",
					Timestamp: 0,
					TracesCnt: cnt,
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
