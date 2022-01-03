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
bool isTokenFunc(const string_q& input) {
    static CStringBoolMap sigs = {
        make_pair("0x095ea7b3", true),  // approve(address spender, uint256 value)
        make_pair("0xa9059cbb", true),  // transfer(address from, uint256 to);
        make_pair("0x23b872dd", true),  // transferFrom(address from, address to, uint256 value)
        make_pair("0xb3e1c718", true),  // _safeMint(address, uint256)
        make_pair("0x6a4f832b", true),  // _safeMint(address, uint256, bytes)
        make_pair("0xa1448194", true),  // safeMint(address, uint256)
        make_pair("0x8832e6e3", true),  // safeMint(address, uint256, bytes)
        make_pair("0x4e6ec247", true),  // _mint(address, uint256)
        make_pair("0x4cd4edcb", true),  // _mint(address, uint256, bytes, bytes)
        make_pair("0x40c10f19", true),  // mint(address, uint256)
        make_pair("0xcfa84fc1", true),  // mint(uint256, address[], uint256[])
        make_pair("0x278d9c41", true),  // mintEventToManyUsers(uint256, address[])
        make_pair("0x78b27221", true),  // mintFungible(uint256, address[], uint256[])
        make_pair("0xf9419088", true),  // mintNonFungible(uint256, address[])
        make_pair("0xf190ac5f", true),  // mintToAddresses(address[], uint256)
        make_pair("0xa140ae23", true),  // mintToken(uint256, address)
        make_pair("0xf980f3dc", true),  // mintUserToManyEvents(uint256[], address)
        make_pair("0x14004ef3", true),  // multimint(address[], uint256[])
        make_pair("0x6a627842", true),  // mint(address)
        make_pair("0xa0712d68", true),  // mint(uint256)
    };
    return sigs[input.substr(0, 10)];
}

//-----------------------------------------------------------------------
#define CTopicBoolMap CStringBoolMap
bool isTokenTopic(const CLogEntry* log) {
    if (!log || log->topics.size() == 0)
        return false;

    static CTopicBoolMap topics = {
        make_pair("0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef",
                  true),  // Transfer(address from, address to, uint256 value)
        make_pair("0x8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925",
                  true),  // Approval(address owner, address spender, uint256 value)
        make_pair("0xd4735d920b0f87494915f556dd9b54c8f309026070caea5c737245152564d266",
                  true),  // Transfer(bytes32 node, address owner)
        make_pair("0x30385c845b448a36257a6a1716e6ad2e1bc2cbe333cde1e69fe849ad6511adfe",
                  true),  // Minted(address,uint256)
    };

    return topics[log->topics[0]];
}

//-----------------------------------------------------------------------
bool COptions::articulateAll(CTransaction& trans) {
    if (articulate) {
        if (!trans.articulatedTx.name.empty()) {
            // LOG_INFO("Short cuircut");
            return false;
        }
        abiMap[trans.to]++;
        if (abiMap[trans.to] == 1 || fileExists(getPathToCache("abis/" + trans.to))) {
            abi_spec.loadAbiFromEtherscan(trans.to);
        }
        abi_spec.articulateTransaction(&trans);
        trans.hasToken |= isTokenFunc(trans.input);

        string_q bytesOnly = substitute(accountedFor.address, "0x", "");
        for (size_t j = 0; j < trans.receipt.logs.size(); j++) {
            CLogEntry* log = (CLogEntry*)&trans.receipt.logs[j];  // NOLINT
            trans.hasToken |= isTokenTopic(log);
            string_q str = log->Format();
            if (contains(str, bytesOnly)) {
                abiMap[log->address]++;
                if (abiMap[log->address] == 1 || fileExists(getPathToCache("abis/" + log->address))) {
                    abi_spec.loadAbiFromEtherscan(log->address);
                }
                abi_spec.articulateLog(log);
            }
        }

        for (size_t j = 0; j < trans.traces.size(); j++) {
            CTrace* trace = (CTrace*)&trans.traces[j];  // NOLINT
            trans.hasToken |= isTokenFunc(trace->action.input);
            abiMap[trace->action.to]++;
            if (abiMap[trace->action.to] == 1 || fileExists(getPathToCache("abis/" + trace->action.to))) {
                abi_spec.loadAbiFromEtherscan(trace->action.to);
            }
            abi_spec.articulateTrace(trace);
        }
    } else {
        // Even if we're not articulating, we want to mark token-related transactions
        trans.hasToken |= isTokenFunc(trans.input);
        for (size_t j = 0; j < trans.receipt.logs.size(); j++) {
            CLogEntry* log = (CLogEntry*)&trans.receipt.logs[j];  // NOLINT
            trans.hasToken |= isTokenTopic(log);
        }
    }
    return true;
}
