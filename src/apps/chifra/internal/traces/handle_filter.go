package tracesPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *TracesOptions) HandleFilter() error {
	abiMap := make(abi.AbiInterfaceMap)
	loadedMap := make(map[types.Address]bool)
	chain := opts.Globals.Chain

	ctx, cancel := context.WithCancel(context.Background())

	// Note: Make sure to add an entry to enabledForCmd in src/apps/chifra/pkg/output/helpers.go
	fetchData := func(modelChan chan types.Modeler[types.RawTrace], errorChan chan error) {
		traces, err := rpcClient.GetTracesByFilter(opts.Globals.Chain, opts.Filter)
		if err != nil {
			errorChan <- err
			cancel()
			return
		}
		if len(traces) == 0 {
			return
		}

		ts := types.Timestamp(0) // rpc.GetBlockTimestamp(opts.Globals.Chain, uint64(traces[0].BlockNumber))
		for _, trace := range traces {
			// Note: This is needed because of a GoLang bug when taking the pointer of a loop variable
			trace := trace
			trace.Timestamp = ts
			var err error
			if !loadedMap[trace.Action.To] {
				if err = abi.LoadAbi(chain, trace.Action.To, abiMap); err != nil {
					// continue processing even with an error
					errorChan <- err
					err = nil
				}
			}
			if err == nil {
				trace.ArticulatedTrace, err = articulate.ArticulateTrace(&trace, abiMap)
				if err != nil {
					// continue processing even with an error
					errorChan <- err
				}
			}
			modelChan <- &trace
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
			"articulate": opts.Articulate,
		},
	})
}

/*
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool once = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;
        if (once)
            cout << exportPreamble(expContext().fmtMap["header"], GETRUNTIME_CLASS(CTrace));
        for (auto f : options.filters) {
            CTraceArray traces;
            getTracesByFilter(traces, f);
            for (auto trace : traces) {
                CBlock block;
                getBlock(block, trace.blockNumber);
                CTransaction trans;
                trans.timestamp = block.timestamp;
                trace.pTransaction = &trans;
                cout << (!options.firstOut ? ", " : "");
                cout << trace << endl;
                options.firstOut = false;
            }
        }
        once = false;
    }
    cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);

    etherlib_cleanup();
    return 0;
}

//-----------------------------------------------------------------------
void getTracesByFilter(CTraceArray& traces, const CTraceFilter& filter) {
    if (filter.Format() == CTraceFilter().Format())
        return;

    string_q toAddrs;
    for (auto addr : filter.toAddress) {
        if (!isZeroAddr(addr))
            toAddrs += ("\"" + addr + "\",");
    }
    if (!toAddrs.empty()) {
        toAddrs = "[" + trim(toAddrs, ',') + "]";
    }

    string_q fromAddrs;
    for (auto addr : filter.fromAddress) {
        if (!isZeroAddr(addr))
            fromAddrs += ("\"" + addr + "\",");
    }
    if (!fromAddrs.empty()) {
        fromAddrs = "[" + trim(fromAddrs, ',') + "]";
    }

#define AA(test, name, val) ((test) ? "\"" + string_q(name) + "\": " + (val) + "," : "")
    string_q params;
    params += AA(filter.fromBlock, "fromBlock", "\"" + uint_2_Hex(filter.fromBlock) + "\"");
    params += AA(filter.toBlock, "toBlock", "\"" + uint_2_Hex(filter.toBlock) + "\"");
    params += AA(!fromAddrs.empty(), "fromAddress", fromAddrs);
    params += AA(!toAddrs.empty(), "toAddress", toAddrs);
    params += AA(filter.after, "after", uint_2_Str(filter.after));
    params += AA(filter.count, "count", uint_2_Str(filter.count));
    params = "{" + trim(params, ',') + "}";

    cerr << substitute(params, " ", "") << endl;

    string_q result = "[" + callRPC("trace_filter", "[" + params + "]", true) + "]";
    CRPCResult generic;
    generic.parseJson3(result);                                   // pull out the result
    generic.result = cleanUpJson((char*)generic.result.c_str());  // NOLINT
    CTrace trace;
    traces.clear();
    while (trace.parseJson4(generic.result)) {
        traces.push_back(trace);
        trace = CTrace();  // reset
    }
}

    if (!filter.empty()) {
        string_q headerLine = "fromBlock,toBlock,fromAddress,toAddress,after,count";
        CStringArray headers;
        explode(headers, headerLine, ',');
        CTraceFilter f;
        string_q line = substitute(filter, "!", ",");
        f.parseCSV(headers, line);
        // block numbers may be hex, number or special, so handle them now
        CStringArray parts;
        explode(parts, filter, '!');
        if (parts.size() > 1)
            extractBlocksFromFilter(f.fromBlock, parts[0], f.toBlock, parts[1]);
        if (f.fromBlock > f.toBlock)
            return usage("filter.fromBlock must be less or equal to filter.toBlock.");
        if (f.fromBlock + 100 < f.toBlock)
            return usage("filter.fromBlock must be no more than 100 blocks before filter.toBlock.");
        filters.push_back(f);
        manageFields("CTraceAction:balance,init,refundAddress,selfDestructed", false);  // hide
        manageFields("CTraceResult:code,address", false);
        manageFields("CTrace:error", false);
        GETRUNTIME_CLASS(CTrace)->sortFieldList();
        GETRUNTIME_CLASS(CTraceAction)->sortFieldList();
        GETRUNTIME_CLASS(CTraceResult)->sortFieldList();
        if (isTestMode() && !isApiMode()) {
            ostringstream os;
            for (auto ff : filters) {
                os << ff << endl;
            }
            LOG_INFO(os.str());
        }
        return true;
    }

bool COptions::extractBlocksFromFilter(blknum_t& b1, const string_q& p1, blknum_t& b2, const string_q& p2) {
    blknum_t latest = getLatestBlock_client();
    COptionsBlockList blocks(this);

    // if p1 is empty, the user provided nothing so just return
    if (p1.empty())
        return true;
    // parse p1 into b1
    if (!parseBlockList2(this, blocks, p1, latest))
        return usage("Could not parse invalid block " + p1 + ".");
    b1 = blocks.numList.size() ? blocks.numList[0] : latest;

    // if p2 is empty, set b2 to b1 and return
    if (p2.empty()) {
        b2 = b1;
        return true;
    }

    if (!parseBlockList2(this, blocks, p2, latest))
        return usage("Could not parse invalid block " + p2 + ".");
    b2 = blocks.numList.size() > 1 ? blocks.numList[1] : b1;

    return true;
}
*/
