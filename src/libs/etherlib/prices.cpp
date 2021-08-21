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

extern const char* STR_GET_RESERVES;
//---------------------------------------------------------------------------
double getPriceInUsd(blknum_t bn, string& priceSource, const address_t& addr) {
    return addr.empty() ? getPriceFromUni_EthUsd(bn, priceSource) : getPriceFromUni_TokEth(bn, priceSource, addr);
}

//---------------------------------------------------------------------------
static const address_t sai = "0x89d24a6b4ccb1b6faa2625fe562bdd9a23260359";
static const address_t dai = "0x6b175474e89094c44da98b954eedeac495271d0f";
static const address_t wEth = "0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2";
static const string_q uniswapFactory = "0x5c69bee701ef814a2b6a3edd4b1652cb9cc5aa6f";
static const string_q getPair = "0xe6a43905";      // getPair(address,address)
static const string_q getReserves = "0x0902f1ac";  // getReserves()
static const string_q reserveBytes = "";

//---------------------------------------------------------------------------
bool findUniPair(CEthCall& pair, const address_t& r1In, const address_t& r2In) {
    if (r1In == r2In)
        return false;

    address_t r1 = r2In > r1In ? r1In : r2In;
    address_t r2 = r2In > r1In ? r2In : r1In;

    static map<string_q, CEthCall> pairs;
    string_q key = r1 + r2;
    CEthCall existing = pairs[key];
    if (!existing.address.empty()) {
        pair = existing;
        return true;
    }

    static map<string_q, bool> notPairs;
    if (notPairs[key])
        return false;

    static CEthCall uniFactory;
    if (uniFactory.address.empty()) {
        uniFactory.address = uniswapFactory;
        uniFactory.encoding = getPair;
        uniFactory.abi_spec.loadAbisKnown("uniswap");
        uniFactory.blockNumber = getBlockProgress(BP_CLIENT).client;  // doesn't really matter
        uniFactory.deployed = getDeployBlock(uniswapFactory);
    }
    uniFactory.bytes = addr_2_Pad64(r1) + addr_2_Pad64(r2);
    if (!doEthCall(uniFactory)) {
        notPairs[key] = true;
        return false;
    }

    pair.address = uniFactory.getResults();
    if (pair.address.empty() || isZeroAddr(pair.address)) {
        notPairs[key] = true;
        return false;
    }

    loadAbiJsonString(STR_GET_RESERVES, pair.abi_spec);
    pair.abi_spec.address = pair.address;
    pair.encoding = getReserves;
    pair.bytes = reserveBytes;
    pair.deployed = getDeployBlock(pair.address);
    pairs[key] = pair;
    return true;
}

//---------------------------------------------------------------------------
bool getPriceFromPair(CEthCall& pair, string_q& priceSource, double& priceOut) {
    priceOut = 1.;

    if (!doEthCall(pair))
        return false;

    CStringArray results;
    if (!pair.getResults(results) || results.size() < 2) {
        LOG_WARN("Could not get results from pair: ", pair.address);
        cerr << pair << endl;
        return false;
    }

    double reserve1 = str_2_Double(wei_2_Ether(str_2_Wei(results[0]), 18));
    double reserve2 = str_2_Double(wei_2_Ether(str_2_Wei(results[1]), 18));
    priceOut = reserve1 / reserve2;
    priceSource = "uniswap";
    return true;
}

//---------------------------------------------------------------------------
double getPriceFromUni_EthUsd(blknum_t bn, string_q& priceSource) {
    CEthCall thePair;
    if (findUniPair(thePair, wEth, dai)) {
        thePair.blockNumber = bn;
        double price;
        if (getPriceFromPair(thePair, priceSource, price))
            return price;
    }

    return getPriceFromMaker_EthUsd(bn, priceSource);
}

//---------------------------------------------------------------------------
double getPriceFromUni_TokEth(blknum_t bn, string_q& priceSource, const address_t& tok) {
    static const CStringArray stableCoins = {
        dai,
        "0xA0b86991c6218b36c1d19D4a2e9Eb0cE3606eB48",  // USDC
        "0xdAC17F958D2ee523a2206206994597C13D831ec7",  // Tether
        "0xdac17f958d2ee523a2206206994597c13d831ec7",  // USDT
        sai,
    };
    for (auto coin : stableCoins) {
        if (coin == tok) {
            priceSource = "stable-coin";
            return 1.;  // Short curcuit...not accuate, but fast
        }
    }

    CEthCall thePair;
    if (findUniPair(thePair, tok, wEth)) {
        thePair.blockNumber = bn;
        double price;
        if (getPriceFromPair(thePair, priceSource, price))
            return price;
    }

    priceSource = "not-priced";
    return 1.;
}

//---------------------------------------------------------------------------
double getPriceFromMaker_EthUsd(blknum_t bn, string_q& priceSource) {
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
            priceSource = "maker";
            return str_2_Uint(wei_2_Str(ether)) / 100.;
        }
    }

    priceSource = "not-priced";
    return 1.;
}

//---------------------------------------------------------------------------
string_q addr_2_Pad64(const address_t& addr) {
    return padLeft(substitute(addr, "0x", ""), 64, '0');
}

//---------------------------------------------------------------------------
const char* STR_GET_RESERVES =
    "{"
    "  \"name\": \"getReserves\","
    "  \"type\": \"function\","
    "  \"signature\": \"getReserves()\","
    "  \"encoding\": \"0x0902f1ac\","
    "  \"inputs\": [],"
    "  \"outputs\":"
    "    ["
    "      {"
    "        \"type\": \"uint112\","
    "        \"name\": \"_reserve0\","
    "        \"internalType\": \"uint112\""
    "      },"
    "      {"
    "        \"type\": \"uint112\","
    "        \"name\": \"_reserve1\","
    "        \"internalType\": \"uint112\""
    "      },"
    "      {"
    "        \"type\": \"uint32\","
    "        \"name\": \"_blockTimestampLast\","
    "        \"internalType\": \"uint32\""
    "      }"
    "    ]"
    "}";

}  // namespace qblocks
