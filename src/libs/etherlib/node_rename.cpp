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
#include <string>
#include "node.h"
#include "filenames.h"

namespace qblocks {

//--------------------------------------------------------------------------------
// TODO: What exactly is this mess? I think it's C++ specific and will disappear in GoLang code
string_q renameExportFields(const string_q& className, const string_q& inStr) {
    string_q ret = inStr;
    replace(ret, "blocknumber", "blockNumber");
    replace(ret, "blockhash", "blockHash");
    replace(ret, "transactionhash", "transactionHash");
    replace(ret, "transactionindex", "transactionIndex");
    replace(ret, "traceindex", "traceIndex");

    // clang-format off
    if (className == "CTransaction") {
        map<string_q, string_q> renames = {
            make_pair("ethergasprice", "ethGasPrice"),
            make_pair("tracecnt", "traceCnt"),
            make_pair("gasused", "gasUsed"),
            make_pair("iserror", "isError"),
            make_pair("compressedtx", "compressedTx"),
        };
        for (auto item : renames)
            replace(ret, item.first, item.second);
        return ret;
    }

    if (className == "CBlock") {
        map<string_q, string_q> renames = {
            make_pair("gasused", "gasUsed"),
            make_pair("gaslimit", "gasLimit"),
            make_pair("basefeepergas", "baseFeePerGas"),
            make_pair("parenthash", "parentHash"),
            make_pair("transactionscnt", "transactionsCnt"),
            make_pair("unclescnt", "unclesCnt"),
            make_pair("addr_count", "addrCount"),
            make_pair("address_count", "addrCount"),
        };
        for (auto item : renames)
            replace(ret, item.first, item.second);
        return ret;
    }

    if (className == "CLog") {
        map<string_q, string_q> renames = {
            make_pair("logindex", "logIndex"),
            make_pair("gasused", "gasUsed"),
            make_pair("compressedlog", "compressedLog"),
        };
        for (auto item : renames)
            replace(ret, item.first, item.second);
        return ret;
    }

    if (className == "CEthCall") {
        map<string_q, string_q> renames = {
            make_pair("compressedresult", "compressedResult"),
        };
        for (auto item : renames)
            replace(ret, item.first, item.second);
        return ret;
    }

    if (className == "CTrace") {
        map<string_q, string_q> renames = {
            make_pair("compressedtrace", "compressedTrace"),
            make_pair("gasused", "gasUsed"),
            make_pair("traceaddress", "traceAddress"),
        };
        for (auto item : renames)
            replace(ret, item.first, item.second);
        return ret;
    }

    if (className == "CReconciliation") {
        map<string_q, string_q> renames = {
            make_pair("assetaddr", "assetAddress"),
            make_pair("assetsymbol", "assetSymbol"),
            make_pair("prevappblk", "prevAppBlk"),
            make_pair("prevbal", "prevBal"),
            make_pair("begbal", "begBal"),
            make_pair("amountin", "amountIn"),
            make_pair("internalin", "internalIn"),
            make_pair("selfdestructin", "selfDestructIn"),
            make_pair("minerbaserewardin", "minerBaseRewardIn"),
            make_pair("minernephewrewardin", "minerNephewRewardIn"),
            make_pair("minertxfeein", "minerTxFeeIn"),
            make_pair("minerunclerewardin", "minerUncleRewardIn"),
            make_pair("prefundin", "prefundIn"),
            make_pair("totalin", "totalIn"),
            make_pair("amountout", "amountOut"),
            make_pair("internalout", "internalOut"),
            make_pair("selfdestructout", "selfDestructOut"),
            make_pair("gasout", "gasOut"),
            make_pair("totalout", "totalOut"),
            make_pair("totaloutlessgas", "totalOutLessGas"),
            make_pair("amountnet", "amountNet"),
            make_pair("endbal", "endBal"),
            make_pair("begbaldiff", "begBalDiff"),
            make_pair("endbalcalc", "endBalCalc"),
            make_pair("endbaldiff", "endBalDiff"),
            make_pair("spotprice", "spotPrice"),
            make_pair("pricesource", "priceSource"),
            make_pair("reconciliationtype", "reconciliationType"),
            make_pair("transactionhash", "transactionHash"),
            make_pair("accountedfor", "accountedFor"),
        };
        for (auto item : renames)
            replace(ret, item.first, item.second);
        return ret;
    }

    return ret;
}

}  // namespace qblocks
