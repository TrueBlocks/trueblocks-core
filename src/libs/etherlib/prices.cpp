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
#include "prices.h"
#include "node.h"

namespace qblocks {

//---------------------------------------------------------------------------
double getPriceInUsd(blknum_t bn, const address_t& addr) {
    if (bn == 0 || !addr.empty())
        return 1.;
    return getPriceFromUni_EthUsd(bn);
}

//---------------------------------------------------------------------------
static const address_t wEth = "0x6b175474e89094c44da98b954eedeac495271d0f";
static const address_t dai = "0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2";
static const string_q uniswapFactory = "0x5c69bee701ef814a2b6a3edd4b1652cb9cc5aa6f";
static const string_q getPair = "0xe6a43905";  // getPair(address,address)
static const string_q pairBytes = addr_2_Pad64(wEth) + addr_2_Pad64(dai);
static const string_q getReserves = "0x0902f1ac";  // getReserves()
static const string_q reserveBytes = "";

//---------------------------------------------------------------------------
double getPriceFromUni_EthUsd(blknum_t bn) {
    static CEthCall theCall2;
    if (theCall2.address.empty()) {
        // Find the pair we need only once to avoid repeatedly querying this
        CEthCall pairCall;
        pairCall.address = uniswapFactory;
        pairCall.encoding = getPair;
        pairCall.bytes = pairBytes;
        pairCall.abi_spec.loadAbisFromKnown();
        pairCall.blockNumber = getBlockProgress(BP_CLIENT).client;
        LOG4("Calling ", substitute(pairCall.Format(), "\n", " "));
        if (doEthCall(pairCall)) {
            theCall2.address = pairCall.getResults();
            theCall2.abi_spec = pairCall.abi_spec;
            LOG_INFO(bGreen, "Found USD Pair: ", theCall2.address, " with ", theCall2.abi_spec.interfaces.size(),
                     " endpoints", cOff);
        } else {
            LOG_WARN(bRed, "Could not find USD Pair: ", theCall2.address, cOff);
        }
        theCall2.encoding = getReserves;
        theCall2.bytes = reserveBytes;
    }
    theCall2.blockNumber = bn;
    if (doEthCall(theCall2) && !theCall2.result.outputs.empty()) {
        CStringArray results;
        if (theCall2.getResults(results) && results.size() > 1) {
            double reserve1 = str_2_Double(wei_2_Ether(str_2_Wei(results[0]), 18));
            double reserve2 = str_2_Double(wei_2_Ether(str_2_Wei(results[1]), 18));
            return reserve1 / reserve2;
        }
    }
    return getPriceFromMaker_EthUsd(bn);
}

//---------------------------------------------------------------------------
double getPriceFromMaker_EthUsd(blknum_t bn) {
    static const char* makerMedianizer = "0x729d19f657bd0614b4985cf1d82531c67569197b";
    static const char* peek = "0x59e02dd7";
    static CAbi spec;
    if (!spec.interfaces.size())
        spec.loadAbiFromEtherscan(makerMedianizer);
    CEthCall theCall;
    theCall.blockNumber = bn;
    theCall.address = makerMedianizer;
    theCall.encoding = peek;
    theCall.abi_spec = spec;
    if (doEthCall(theCall)) {
        CStringArray results;
        theCall.getResults(results);
        if (results.size() > 1 && results[1] == "true") {
            string_q hex = results[0];
            wei_t wei = str_2_Wei(hex) * 100;
            wei_t ether = wei / weiPerEther();
            return str_2_Uint(wei_2_Str(ether)) / 100.;
        }
    }
    return 1.;
}

//---------------------------------------------------------------------------
string_q addr_2_Pad64(const address_t& addr) {
    return padLeft(substitute(addr, "0x", ""), 64, '0');
}
}  // namespace qblocks
