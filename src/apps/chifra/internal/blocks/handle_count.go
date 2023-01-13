// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package blocksPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *BlocksOptions) HandleCounts() error {
	// Don't do this in the loop
	meta, err := rpcClient.GetMetaData(opts.Globals.Chain, opts.Globals.TestMode)
	if err != nil {
		return err
	}
	if opts.Globals.TestMode {
		meta.Latest = 2000100
	}

	ctx, cancel := context.WithCancel(context.Background())

	// Note: Make sure to add an entry to enabledForCmd in src/apps/chifra/pkg/output/helpers.go
	fetchData := func(modelChan chan types.Modeler[types.RawBlockCount], errorChan chan error) {
		for _, br := range opts.BlockIds {
			blockNums, err := br.ResolveBlocks(opts.Globals.Chain)
			if err != nil {
				errorChan <- err
				cancel()
				return
			}

			for _, bn := range blockNums {
				finalized := meta.Age(bn) > 28
				var block types.SimpleBlock[string]
				if block, err = rpcClient.GetBlockByNumber(opts.Globals.Chain, bn, finalized); err != nil {
					errorChan <- err
					cancel()
					return
				}

				blockCount := types.SimpleBlockCount{
					BlockNumber:     block.BlockNumber,
					Timestamp:       uint64(block.Timestamp),
					TransactionsCnt: uint64(len(block.Transactions)),
				}

				if opts.Uncles {
					if blockCount.UnclesCnt, err = rpcClient.GetUncleCountByNumber(opts.Globals.Chain, bn); err != nil {
						errorChan <- err
						cancel()
						return
					}
				}

				if opts.Trace {
					if blockCount.TracesCnt, err = rpcClient.GetTraceCountByNumber(opts.Globals.Chain, bn); err != nil {
						errorChan <- err
						cancel()
						return
					}
				}

				modelChan <- &blockCount
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
		Extra: map[string]interface{}{
			"count":     opts.Count,
			"uncles":    opts.Uncles,
			"traces":    opts.Trace,
			"addresses": opts.Uniq || opts.Apps,
		},
	})
}

// // getTraceCount
// func getTraceCount(bn uint64) (uint64, error) {
// 	var traces rpcClient.Traces
// 	tracePayload := rpcClient.RPCPayload{
// 		Method:    "trace_block",
// 		RPCParams: rpcClient.RPCParams{fmt.Sprintf("0x%x", bn)},
// 	}
// 	err := rpcClient.FromRpc("http://localhost:23456", &tracePayload, &traces)
// 	if err != nil {
// 		// fmt.Println("rpcCall failed at block", blockNum, err)
// 		return utils.NOPOS, err
// 	}
// 	return uint64(len(traces.Result)), nil
// }
