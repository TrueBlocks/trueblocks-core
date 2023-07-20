// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tracesPkg

import (
	"context"
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *TracesOptions) HandleShowTraces() error {
	abiCache := articulate.NewAbiCache()
	var rpcOptions *rpcClient.Options = rpcClient.NoOptions
	chain := opts.Globals.Chain

	ctx, cancel := context.WithCancel(context.Background())
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
				traces, err := rpcClient.GetTracesByTransactionId(opts.Globals.Chain, uint64(id.BlockNumber), uint64(id.TransactionIndex), rpcOptions)
				if err != nil {
					errorChan <- err
					if errors.Is(err, ethereum.NotFound) {
						continue
					}
					cancel()
					return
				}

				for index := range traces {
					// Note: This is needed because of a GoLang bug when taking the pointer of a loop variable
					traces[index].Timestamp = ts
					if opts.Articulate {
						if err = abiCache.ArticulateTrace(chain, &traces[index]); err != nil {
							errorChan <- err // continue even with an error
						}
					}
					modelChan <- &traces[index]
				}
			}
		}
	}

	extra := map[string]interface{}{
		"articulate": opts.Articulate,
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}

// See issue #2791 - This is the code that used to generate extra traces to make reconcilation work
// (or, at least, similar code in `chifra export` generated these traces.
// bool isSuicide = trace.action.selfDestructed != "";
// bool isCreation = trace.result.address != "";
// // do not colapse
// if (isCreation) {
//   displayAsTrace(opt, trace);
//   displayAsCreation(opt, trace);
// }
// // do not colapse
// if (isSuicide) {
//   displayAsSuicide(opt, trace);
// }
// // do not colapse
// if (!isCreation && !isSuicide) {
//   displayAsTrace(opt, trace);
// }
//--------------------------------------------------------------
// bool displayAsCreation(COptions* opt, const CTrace& trace) {
//     if (trace.result.address == "")
//         return false;
//     CTrace copy = trace;
//     copy.action.from = "0x0";
//     copy.action.to = trace.result.address;
//     copy.action.callType = "creation";
//     copy.action.value = trace.action.value;
//     if (copy.traceAddress.size() == 0)
//         copy.traceAddress.push_back("null");
//     copy.traceAddress.push_back("s");
//     copy.transactionHash = uint_2_Hex(trace.blockNumber * 100000 + trace.transactionIndex);
//     copy.action.input = trace.action.input;
//     return displayAsTrace(opt, copy);
// }
// //--------------------------------------------------------------
// bool displayAsSuicide(COptions* opt, const CTrace& trace) {
//     if (trace.action.refundAddress == "")
//         return false;
//     CTrace copy = trace;
//     copy.action.from = trace.action.selfDestructed;
//     copy.action.to = trace.action.refundAddress;
//     copy.action.callType = "suicide";
//     copy.action.value = trace.action.balance;
//     copy.traceAddress.push_back("s");
//     copy.transactionHash = uint_2_Hex(trace.blockNumber * 100000 + trace.transactionIndex);
//     copy.action.input = "0x";
//     return displayAsTrace(opt, copy);
// }
// //--------------------------------------------------------------
// bool displayAsTrace(COptions* opt, const CTrace& trace) {
//     bool isText = (expContext().exportFmt & (TXT1 | CSV1));
//     if (isText) {
//         cout << trim(trace.Format(expContext().fmtMap["format"]), '\t') << endl;
//     } else {
//         if (!opt->firstOut)
//             cout << ",";
//         cout << "  ";
//         indent();
//         trace.toJson(cout);
//         unindent();
//         opt->firstOut = false;
//     }
//     return true;
// }
