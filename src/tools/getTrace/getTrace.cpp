/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//--------------------------------------------------------------
int main(int argc, const char *argv[]) {
    // Tell the system where the blocks are and which version to use
    setStorageRoot(BLOCK_CACHE);
    etherlib_init("binary");

    CRPCResult::registerClass();
    CTrace::registerClass();
    CTraceAction::registerClass();
    CTraceResult::registerClass();

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    expContext().spcs = 4;
    while (!options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        SFString fmt = "";
        if (options.terse) {
            fmt += "\"blockHash\": \"[{BLOCKHASH}]\", ";
            fmt += "\"blockNumber\": \"[{BLOCKNUMBER}]\", ";
//            fmt += "\"subtraces\": \"[{SUBTRACES}]\", ";
            fmt += "\"traceAddress\": \"[{TRACEADDRESS}]\", ";
            fmt += "\"transactionHash\": \"[{TRANSACTIONHASH}]\", ";
            fmt += "\"transactionPosition\": \"[{TRANSACTIONPOSITION}]\", ";
            fmt += "\"type\": \"[{TYPE}]\", ";
            fmt += "\"action\": {\n";
            fmt += SUBFIELD_FMT("action", "callType", true);
            fmt += SUBFIELD_FMT("action", "from", true);
            fmt += SUBFIELD_FMT("action", "gas", true);
            fmt += SUBFIELD_FMT("action", "input", true);
            fmt += SUBFIELD_FMT("action", "to", true);
            fmt += SUBFIELD_FMT("action", "value", false);
            fmt += "},\n";
            fmt += "\"result\": {\n";
            fmt += SUBFIELD_FMT("result", "gasUsed", true);
            fmt += SUBFIELD_FMT("result", "output", false);
            fmt += "}\n";
        }

        if (options.raw) {

            SFString trace;
            queryRawTrace(trace, options.hash);
            cout << trace << "\n";

        } else {
            CTraceArray traces;
            getTraces(traces, options.hash);

            cout << "[";
            for (uint32_t i = 0 ; i < traces.getCount() ; i++) {
                SFString res = traces[i].Format(fmt).Substitute(", }", " }");
                if (!fmt.empty())
                    res = "{ " + res + " }";
                cout << res << (i < traces.getCount()-1 ? ",\n" : "");
            }
            cout << "]\n";
        }
    }
    return 0;
}
