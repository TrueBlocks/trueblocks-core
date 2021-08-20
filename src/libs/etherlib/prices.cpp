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
    if (bn == 0)
        return 1.;
    return addr.empty() ? getPriceFromUni_EthUsd(bn) : getPriceFromUni_TokEth(bn, addr);
}

//---------------------------------------------------------------------------
static const address_t dai = "0x6b175474e89094c44da98b954eedeac495271d0f";
static const address_t wEth = "0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2";
static const string_q uniswapFactory = "0x5c69bee701ef814a2b6a3edd4b1652cb9cc5aa6f";
static const string_q getPair = "0xe6a43905";      // getPair(address,address)
static const string_q getReserves = "0x0902f1ac";  // getReserves()
static const string_q reserveBytes = "";

//---------------------------------------------------------------------------
bool findUniPair(CEthCall& pair, const address_t& r1, const address_t& r2) {
    static map<string_q, CEthCall> pairs;
    string_q key = r1 + r2;
    CEthCall existing = pairs[key];
    if (!existing.address.empty()) {
        pair = existing;
        return true;
    }

    static CEthCall uniFactory;
    if (uniFactory.address.empty()) {
        uniFactory.address = uniswapFactory;
        uniFactory.encoding = getPair;
        uniFactory.abi_spec.loadAbisFromKnown("uniswap");
        uniFactory.blockNumber = getBlockProgress(BP_CLIENT).client;  // doesn't really matter
    }
    uniFactory.bytes = addr_2_Pad64(r1) + addr_2_Pad64(r2);
    if (!doEthCall(uniFactory)) {
        return false;
    }

    pair.address = uniFactory.getResults();
    if (pair.address.empty() || isZeroAddr(pair.address)) {
        LOG4("Could not fund uniswap pair for [", r1, "]-->[", r2, "]");
        return false;
    }
    LOG4("Found uniswap pair for [", r1, "]-->[", r2, "] at ", pair.address);
    pair.abi_spec.loadAbisFromKnown("uniswap");
    pair.encoding = getReserves;
    pair.bytes = reserveBytes;
    pairs[key] = pair;
    return true;
}

//---------------------------------------------------------------------------
bool getPriceFromPair(CEthCall& pair, double& priceOut) {
    priceOut = 1.;

    if (!doEthCall(pair))
        return false;

    if (pair.result.outputs.empty())
        return false;

    CStringArray results;
    if (!pair.getResults(results) || results.size() < 2)
        return false;

    double reserve1 = str_2_Double(wei_2_Ether(str_2_Wei(results[0]), 18));
    double reserve2 = str_2_Double(wei_2_Ether(str_2_Wei(results[1]), 18));
    priceOut = reserve1 / reserve2;
    return true;
}

//---------------------------------------------------------------------------
double getPriceFromUni_EthUsd(blknum_t bn) {
    CEthCall thePair;
    if (findUniPair(thePair, wEth, dai)) {
        thePair.blockNumber = bn;
        double price;
        if (getPriceFromPair(thePair, price))
            return price;
    }
    return getPriceFromMaker_EthUsd(bn);
}

//---------------------------------------------------------------------------
double getPriceFromUni_TokEth(blknum_t bn, const address_t& tok) {
    if (tok == wEth)
        return 1.;

    CEthCall thePair;
    if (findUniPair(thePair, tok, wEth)) {
        thePair.blockNumber = bn;
        double price;
        if (getPriceFromPair(thePair, price))
            return price;
    }
    return 1.;
}

//---------------------------------------------------------------------------
double getPriceFromMaker_EthUsd(blknum_t bn) {
    static const char* makerMedianizer = "0x729d19f657bd0614b4985cf1d82531c67569197b";
    static const char* peek = "0x59e02dd7";
    static CAbi spec;
    if (!spec.nInterfaces())
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
