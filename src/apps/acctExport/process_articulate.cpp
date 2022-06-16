/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool COptions::articulateAll(CTransaction& trans) {
    if (articulate) {
        if (!trans.articulatedTx.name.empty()) {
            // Already articulated
            return false;
        }

        // Note: here and below, we note that we've seen the 'to' address and if this is the
        // first time we've seen it (or we have the ABI file locally), load it. Note the call to
        // loadAbiFromEtherscan won't redunantly load the file if it's already loaded
        abiMap[trans.to]++;
        if (abiMap[trans.to] == 1 || fileExists(cacheFolder_abis + trans.to + ".json")) {
            abi_spec.loadAbiFromEtherscan(trans.to);
        }
        abi_spec.articulateTransaction(&trans);
        // we want to know if we're a token
        trans.hasToken |= isTokenRelated(trans.input.substr(0, 10));

        string_q bytesOnly = substitute(accountedFor.address, "0x", "");

        // Do the same for the logs...
        for (size_t j = 0; j < trans.receipt.logs.size(); j++) {
            CLogEntry* log = (CLogEntry*)&trans.receipt.logs[j];  // NOLINT
            string_q str = log->Format();
            if (contains(str, bytesOnly)) {
                abiMap[log->address]++;
                if (abiMap[log->address] == 1 || fileExists(cacheFolder_abis + log->address + ".json")) {
                    abi_spec.loadAbiFromEtherscan(log->address);
                }
                abi_spec.articulateLog(log);
            }
            trans.hasToken |= isTokenRelated(log->topics[0]);
        }

        // And the same for the traces...
        for (size_t j = 0; j < trans.traces.size(); j++) {
            CTrace* trace = (CTrace*)&trans.traces[j];  // NOLINT
            trans.hasToken |= isTokenRelated(trace->action.input.substr(0, 10));
            abiMap[trace->action.to]++;
            if (abiMap[trace->action.to] == 1 || fileExists(cacheFolder_abis + trace->action.to + ".json")) {
                abi_spec.loadAbiFromEtherscan(trace->action.to);
            }
            abi_spec.articulateTrace(trace);
        }

    } else {
        // Even if we're not articulating, we want to mark token-related transactions
        trans.hasToken |= isTokenRelated(trans.input.substr(0, 10));
        for (size_t j = 0; j < trans.receipt.logs.size(); j++) {
            CLogEntry* log = (CLogEntry*)&trans.receipt.logs[j];  // NOLINT
            trans.hasToken |= isTokenRelated(log->topics[0]);     // always has at least the topic
        }
    }
    return true;
}
