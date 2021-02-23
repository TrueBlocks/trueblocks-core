/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool isTokenFunc(const string_q& input) {
    CStringArray sigs = {
        "0x095ea7b3",
        "0xa9059cbb",
        "0x23b872dd",
    };
    string_q sig = input.substr(0, 10);
    return sig == sigs[0] || sig == sigs[1] || sig == sigs[2];
}

//-----------------------------------------------------------------------
bool isTokenTopic(const topic_t& topic) {
    static const topic_t transferTopic =
        str_2_Topic("0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef");
    static const topic_t approvalTopic =
        str_2_Topic("0x8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925");
    static const topic_t ensTransferTopic =
        str_2_Topic("0xd4735d920b0f87494915f556dd9b54c8f309026070caea5c737245152564d266");
    return (topic == transferTopic || topic == ensTransferTopic || topic == approvalTopic);
}

//-----------------------------------------------------------------------
void COptions::articulateAll(CTransaction& trans) {
    if (articulate) {
        abiMap[trans.to]++;
        if (abiMap[trans.to] == 1 || fileExists(getCachePath("abis/" + trans.to))) {
            abi_spec.loadAbiFromEtherscan(trans.to);
        }
        abi_spec.articulateTransaction(&trans);
        trans.hasToken |= isTokenFunc(trans.input);

        for (size_t j = 0; j < trans.receipt.logs.size(); j++) {
            CLogEntry* log = (CLogEntry*)&trans.receipt.logs[j];  // NOLINT
            trans.hasToken |= isTokenTopic(log->topics[0]);
            string_q str = log->Format();
            if (contains(str, bytesOnly)) {
                abiMap[log->address]++;
                if (abiMap[log->address] == 1 || fileExists(getCachePath("abis/" + log->address))) {
                    abi_spec.loadAbiFromEtherscan(log->address);
                }
                abi_spec.articulateLog(log);
            }
        }

        for (size_t j = 0; j < trans.traces.size(); j++) {
            CTrace* trace = (CTrace*)&trans.traces[j];  // NOLINT
            trans.hasToken |= isTokenFunc(trace->action.input);
            abiMap[trace->action.to]++;
            if (abiMap[trace->action.to] == 1 || fileExists(getCachePath("abis/" + trace->action.to))) {
                abi_spec.loadAbiFromEtherscan(trace->action.to);
            }
            abi_spec.articulateTrace(trace);
        }
    }
}
