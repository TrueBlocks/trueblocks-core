/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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

//-------------------------------------------------------------------------
void COptions::reportByParts(void) {
    // In order to report by parts, we need to have the abi cached (fail silently)
    for (auto token : tokens) {
        CAbi unused;
        CStringArray unused2;
        loadAbiAndCache(unused, token.address, false, unused2);
    }
    HIDE_FIELD(CAccountWatch, "abi_spec");
    HIDE_FIELD(CAccountWatch, "statement");
    SHOW_FIELD(CTokenState_erc20, "symbol");

//    cout << "[";
    bool first = true;
    for (auto token : tokens) {
        token.abi_spec.loadAbiByAddress(token.address);
        string_q blocks = getBlockNumList();
        while (!blocks.empty()) {
            blknum_t blockNum = str_2_Uint(nextTokenClear(blocks, '|'));
            CStringArray fields = {"name", "symbol", "totalSupply", "decimals", "version"};
            for (auto field : fields) {
                token.setValueByName(field, getTokenState(field, token, blockNum));
            }
//            if (!first)
//                cout << ",";
            cout << token.Format("[{ADDRESS}]\t[{NAME}]\t[{SYMBOL}]\t[{DECIMALS}]") << endl;
            first = false;
        }
    }
//    cout << "]" << endl;
}

namespace qblocks {
//-------------------------------------------------------------------------
string_q doEthCall(const address_t& to, const string_q& encoding, const string_q& bytes, blknum_t blockNum,
                   const CAbi& abi) {
    ostringstream cmd;
    cmd << "[{";
    cmd << "\"to\": \"" << to << "\", ";
    cmd << "\"data\": \"" << encoding << bytes << "\"";
    cmd << "}, \"" << uint_2_Hex(blockNum) << "\"]";

    CFunction ret;
    abi.articulateOutputs(encoding, callRPC("eth_call", cmd.str(), false), ret);
    return ret.outputs.size() ? ret.outputs[0].value : "";
}

//-------------------------------------------------------------------------
string_q getTokenBalanceOf(const CTokenState_erc20& token, const address_t& holder, blknum_t blockNum) {
    return doEthCall(token.address, "0x70a08231", padLeft(extract(holder, 2), 64, '0'), blockNum, token.abi_spec);
}

//-------------------------------------------------------------------------
string_q getTokenState(const string_q& what, const CTokenState_erc20& token, blknum_t blockNum) {
    map<string_q, string_q> sigMap;
    sigMap["totalSupply"] = "0x18160ddd";
    sigMap["decimals"] = "0x313ce567";
    sigMap["version"] = "0x54fd4d50";
    sigMap["symbol"] = "0x95d89b41";
    sigMap["name"] = "0x06fdde03";

    return doEthCall(token.address, sigMap[what], "", blockNum, token.abi_spec);
}
}  // namespace qblocks
