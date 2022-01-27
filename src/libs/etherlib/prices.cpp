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
extern double getPrice_UsdPerEth(blknum_t bn, string_q& priceSource);
extern double getPrice_UsdPerTok(blknum_t bn, string_q& priceSource, const address_t& address);
extern double getPrice_EthPerTok(blknum_t bn, string_q& priceSource, const address_t& address);
extern double getPriceMaker_UsdPerEth(blknum_t bn, string_q& priceSource);

//---------------------------------------------------------------------------
// BOGUS - CHAIN SPECIFIC - UNISWAP PRICING
static const address_t sai = "0x89d24a6b4ccb1b6faa2625fe562bdd9a23260359";
static const address_t dai = "0x6b175474e89094c44da98b954eedeac495271d0f";
static const address_t wEth = "0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2";
static const string_q uniswapFactory = "0x5c69bee701ef814a2b6a3edd4b1652cb9cc5aa6f";
// deployed 10000835
// v3 factory 0x1f98431c8ad98523631ae4a59f267346ea31f984
// deployed 12369621
static const string_q getPair = "0xe6a43905";      // getPair(address,address)
static const string_q getReserves = "0x0902f1ac";  // getReserves()
static const string_q reserveBytes = "";
extern const char* STR_GET_RESERVES;

//---------------------------------------------------------------------------
class CUniPair : public CEthCall {
  public:
    address_t r1;
    address_t r2;
    bool reversed;
    bool selfie;
    CUniPair(const address_t& r1In, const address_t& r2In) : r1(r1In), r2(r2In), reversed(false), selfie(false) {
        if (r1In > r2In) {
            r1 = r2In;
            r2 = r1In;
            reversed = true;
        } else if (r1In == r2In) {
            selfie = true;
        }
    }
    CUniPair(void) : reversed(false), selfie(false) {
    }
    CUniPair(const CUniPair& p) {
        r1 = p.r1;
        r2 = p.r2;
        reversed = p.reversed;
        selfie = p.selfie;
    }
    CUniPair& operator=(const CUniPair& p) {
        r1 = p.r1;
        r2 = p.r2;
        reversed = p.reversed;
        selfie = p.selfie;
        return *this;
    };
    bool findPair(void);
    bool getPrice(blknum_t bn, string_q& priceSource, double& priceOut);
};

//---------------------------------------------------------------------------
double getPriceInUsd(blknum_t bn, string& priceSource, const address_t& addr) {
    return addr.empty() ? getPrice_UsdPerEth(bn, priceSource) : getPrice_UsdPerTok(bn, priceSource, addr);
}

//---------------------------------------------------------------------------
bool CUniPair::findPair(void) {
    if (selfie)
        return true;

    static map<string_q, CUniPair> pairs;
    string_q key = r1 + r2;
    const CUniPair existing = pairs[key];
    if (!existing.address.empty()) {
        *this = existing;
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
    uniFactory.bytes = hex_2_Pad64(r1) + hex_2_Pad64(r2);
    if (!doEthCall(uniFactory)) {
        notPairs[key] = true;
        return false;
    }

    address = uniFactory.getCallResult();
    if (address.empty() || isZeroAddr(address)) {
        notPairs[key] = true;
        return false;
    }

    loadAbiJsonString(STR_GET_RESERVES, abi_spec);
    abi_spec.address = address;
    encoding = getReserves;
    bytes = reserveBytes;
    deployed = getDeployBlock(address);
    pairs[key] = *this;
    return true;
}

//---------------------------------------------------------------------------
bool CUniPair::getPrice(blknum_t bn, string_q& priceSource, double& priceOut) {
    priceOut = 1.;

    blockNumber = bn;
    if (!doEthCall(*this))
        return false;

    CStringArray results;
    if (!getCallResult(results) || results.size() < 2) {
        LOG_WARN("Could not get results from pair: ", address);
        cerr << *this << endl;
        return false;
    }

    double reserve1 = str_2_Double(wei_2_Ether(str_2_Wei(results[0]), 18));
    double reserve2 = str_2_Double(wei_2_Ether(str_2_Wei(results[1]), 18));
    if (reversed) {
        if (reserve1 != 0.0) {
            priceOut = reserve2 / reserve1;
            priceSource = "uniswap";
        }
    } else {
        if (reserve2 != 0.0) {
            priceOut = reserve1 / reserve2;
            priceSource = "uniswap";
        }
    }
    LOG4("r1: ", r1, " r2: ", r2);
    LOG4("reserve1: ", reserve1, " reserve2: ", reserve2, " priceOut: ", priceOut, " reversed: ", reversed);
    return true;
}

//---------------------------------------------------------------------------
double getPrice_UsdPerEth(blknum_t bn, string_q& priceSource) {
    CUniPair usdEth(dai, wEth);
    if (usdEth.findPair()) {
        double price;
        if (usdEth.getPrice(bn, priceSource, price))
            return price;
    }
    return getPriceMaker_UsdPerEth(bn, priceSource);
}

//---------------------------------------------------------------------------
double getPrice_UsdPerTok(blknum_t bn, string_q& priceSource, const address_t& tok) {
    static const CStringArray stableCoins = {
        dai,
        "0xa0b86991c6218b36c1d19d4a2e9eb0ce3606eb48",  // USDC
        "0xdac17f958d2ee523a2206206994597c13d831ec7",  // Tether
        "0xdac17f958d2ee523a2206206994597c13d831ec7",  // USDT
        sai,
    };
    for (auto coin : stableCoins) {
        if (coin % tok) {
            priceSource = "stable-coin";
            return 1.;  // Short curcuit...not accuate, but fast
        }
    }

    if (tok % wEth)
        return getPrice_UsdPerEth(bn, priceSource);

    double ethPerTok = getPrice_EthPerTok(bn, priceSource, tok);
    if (ethPerTok == 0.)
        return 0.;
    double usdPerEth = getPrice_UsdPerEth(bn, priceSource);
    LOG4("ethPerTok: ", ethPerTok, " usdPerEth: ", usdPerEth, " price: ", (usdPerEth * ethPerTok));
    return usdPerEth * ethPerTok;
}

//---------------------------------------------------------------------------
double getPrice_EthPerTok(blknum_t bn, string_q& priceSource, const address_t& tok) {
    CUniPair thePair(wEth, tok);
    if (thePair.findPair()) {
        double price;
        if (thePair.getPrice(bn, priceSource, price))
            return price;
    }

    priceSource = "not-priced";
    return 0.;
}

//---------------------------------------------------------------------------
double getPriceMaker_UsdPerEth(blknum_t bn, string_q& priceSource) {
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
        theCall.getCallResult(results);
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
