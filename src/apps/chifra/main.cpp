/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

#define RETURN(a)                                                                                                      \
    bool ret = (a);                                                                                                    \
    if (!getEnvStr("TEST_MODE").empty()) {                                                                             \
        EXIT_NOMSG(0);                                                                                                 \
    } else {                                                                                                           \
        EXIT_NOMSG(ret);                                                                                               \
    }

//--------------------------------------------------------------
int main(int argc, const char* argv[]) {
    nodeNotRequired();  // not every command needs a node
    acctlib_init(quickQuitHandler);

    ENTER("chifra");
    COptions options;
    if (!options.prepareArguments(argc, argv)) {
        RETURN(1);
    }

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command)) {
            RETURN(1);
        }

        if (options.mode == "list" || options.mode == "export") {
            RETURN(options.handle_export())  //
        } else if (options.mode == "scrape") {
            RETURN(options.handle_scrape())  //
        } else if (options.mode == "rm") {
            RETURN(options.handle_rm())  //
        } else {
            map<string, string> cmdMap;
            cmdMap["status"] = "cacheStatus";
            cmdMap["names"] = "ethNames";
            cmdMap["abis"] = "grabABI";
            cmdMap["slurp"] = "ethSlurp";
            cmdMap["state"] = "getState";
            cmdMap["tokens"] = "getTokenInfo";
            cmdMap["blocks"] = "getBlock";
            cmdMap["transactions"] = "getTrans";
            cmdMap["receipts"] = "getReceipt";
            cmdMap["logs"] = "getLogs";
            cmdMap["traces"] = "getTrace";
            cmdMap["quotes"] = "ethQuote";
            cmdMap["when"] = "whenBlock";
            cmdMap["where"] = "whereBlock";
            if (cmdMap[options.mode] != "") {
                ostringstream os;
                os << cmdMap[options.mode] << " " << substitute(options.tool_flags, "--names", "");
                for (auto addr : options.addrs)
                    os << " " << addr;
                LOG_CALL(os.str());
                // clang-format off
                if (system(os.str().c_str())) {}  // Don't remove cruft. Silences compiler warnings
                // clang-format on
            } else {
                cerr << "Should not happen.";
            }
        }
    }

    acctlib_cleanup();
    EXIT_NOMSG(0);
}
