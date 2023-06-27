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
extern double getPrice_UsdPerEth(string_q& priceSource, blknum_t bn);
extern double getPrice_UsdPerTok(const address_t& assetAddr, string_q& priceSource, blknum_t bn);
extern double getPrice_EthPerTok(const address_t& assetAddr, string_q& priceSource, blknum_t bn);
extern double getPriceMaker_UsdPerEth(string_q& priceSource, blknum_t bn);

//---------------------------------------------------------------------------
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
    address_t which;
    string_q sym;
    CUniPair(blknum_t bn, const address_t& r1In, const address_t& r2In)
        : CEthCall(), r1(r1In), r2(r2In), reversed(false), selfie(false) {
        blockNumber = bn;
        if (r1In > r2In) {
            r1 = r2In;
            r2 = r1In;
            reversed = true;
        } else if (r1In == r2In) {
            selfie = true;
        }
    }
    CUniPair(void) : CEthCall(), reversed(false), selfie(false) {
    }
    CUniPair(const CUniPair& p) {
        CEthCall::initialize();
        r1 = p.r1;
        r2 = p.r2;
        reversed = p.reversed;
        selfie = p.selfie;
    }
    CUniPair& operator=(const CUniPair& p) {
        CEthCall::operator=(p);
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
double getPriceInUsd(const address_t& assetAddr, string& priceSource, blknum_t bn) {
    // TODO: Multi-chain missing feature on other chains
    if (getChain() != "mainnet") {
        return 1.0;
    }
    return assetAddr.empty() || assetAddr == FAKE_ETH_ADDRESS ? getPrice_UsdPerEth(priceSource, bn)
                                                              : getPrice_UsdPerTok(assetAddr, priceSource, bn);
}

//--------------------------------------------------------------
static blknum_t findCodeAt_binarySearch(const address_t& addr, blknum_t first, blknum_t last) {
    if (last > first) {
        size_t mid = first + ((last - first) / 2);
        bool atMid = isContractAt(addr, mid);
        bool atMid1 = isContractAt(addr, mid + 1);
        if (!atMid && atMid1)
            return mid;  // found it
        if (atMid) {
            // we're too high, so search below
            return findCodeAt_binarySearch(addr, first, mid - 1);
        }
        // we're too low, so search above
        return findCodeAt_binarySearch(addr, mid + 1, last);
    }
    return first;
}

static map<address_t, blknum_t> deployMap;
//-------------------------------------------------------------------------------------
blknum_t getDeployBlock(const address_t& addr) {
    if (deployMap[addr] > 0) {
        return deployMap[addr];
    }

    blknum_t latest = getLatestBlock_client();
    if (!isContractAt(addr, latest))
        return NOPOS;
    blknum_t num = findCodeAt_binarySearch(addr, 0, latest);
    if (num) {
        deployMap[addr] = (num + 1);
    }
    return (num ? num + 1 : NOPOS);
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
        uniFactory.blockNumber = blockNumber;
        uniFactory.deployed = getDeployBlock(uniswapFactory);
    }
    uniFactory.bytes = hex_2_Pad64(r1) + hex_2_Pad64(r2);
    if (!doEthCall(uniFactory, true /* proxy */)) {
        if (isTestMode()) {
            cerr << "TEST[DATE|TIME] "
                 << "doEthCall in CUniPrice::findPair returned false" << endl;
        }
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

#define LOG_ONE(a, b)                                                                                                  \
    {                                                                                                                  \
        string_q ttt = ":";                                                                                            \
        cerr << padRight("TEST[DATE|TIME]", 16) << padRight(((a) + ttt), 20) << " " << (b) << endl;                    \
    }

//---------------------------------------------------------------------------
bool CUniPair::getPrice(blknum_t bn, string_q& priceSource, double& priceOut) {
    priceOut = 1.;

    blockNumber = bn;
    if (!doEthCall(*this, true /* proxy */)) {
        if (isTestMode()) {
            LOG_INFO("doEthCall in CUniPrice::getPrice returned false");
        }
        return false;
    }

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

    if (isTestMode()) {
        ostringstream os;
        os << "getPair(" << r1 << ", " << r2 << ")";
        cerr << "TEST[DATE|TIME] =========================================================" << endl;
        cerr << "TEST[DATE|TIME] ===> PRICING FOR " << which << " (" << sym << ")" << endl;
        cerr << "TEST[DATE|TIME] =========================================================" << endl;
        LOG_ONE("blockNumber", bn);
        LOG_ONE("uniswapFactoryV2", uniswapFactory);
        LOG_ONE("theCall", os.str());
        LOG_ONE("pairAddress", address);
        LOG_ONE("theCall", "getReserves()");
        LOG_ONE("first", r1);
        LOG_ONE("second", r2);
        LOG_ONE("reversed", (reversed ? "true" : "false"));
        LOG_ONE("reserve0", double_2_Str(reserve1));
        LOG_ONE("reserve1", double_2_Str(reserve2));
        LOG_ONE("price", double_2_Str(priceOut));
    }

    return true;
}

//---------------------------------------------------------------------------
double getPrice_UsdPerEth(string_q& priceSource, blknum_t bn) {
    CUniPair usdEth(bn, dai, wEth);
    if (usdEth.findPair()) {
        double price;
        usdEth.which = FAKE_ETH_ADDRESS;
        usdEth.sym = "WEI";
        if (usdEth.getPrice(bn, priceSource, price))
            return price;
    }
    return getPriceMaker_UsdPerEth(priceSource, bn);
}

//---------------------------------------------------------------------------
double getPrice_UsdPerTok(const address_t& assetAddr, string_q& priceSource, blknum_t bn) {
    static const CStringArray stableCoins = {
        dai,
        "0xa0b86991c6218b36c1d19d4a2e9eb0ce3606eb48",  // USDC
        "0xdac17f958d2ee523a2206206994597c13d831ec7",  // Tether
        "0xdac17f958d2ee523a2206206994597c13d831ec7",  // USDT
        sai,
    };
    for (auto coin : stableCoins) {
        if (coin % assetAddr) {
            priceSource = "stable-coin";
            return 1.;  // Short curcuit...not accuate, but fast
        }
    }

    if (assetAddr % wEth)
        return getPrice_UsdPerEth(priceSource, bn);

    double ethPerTok = getPrice_EthPerTok(assetAddr, priceSource, bn);
    if (ethPerTok == 0.)
        return 0.;
    double usdPerEth = getPrice_UsdPerEth(priceSource, bn);
    if (isTestMode()) {
        LOG_ONE("ethPerTok", ethPerTok);
        LOG_ONE("usdPerEth", usdPerEth);
        LOG_ONE("price", (usdPerEth * ethPerTok));
    }
    return usdPerEth * ethPerTok;
}

//---------------------------------------------------------------------------
double getPrice_EthPerTok(const address_t& tokenAddr, string_q& priceSource, blknum_t bn) {
    CUniPair thePair(bn, wEth, tokenAddr);
    if (thePair.findPair()) {
        double price;
        thePair.which = tokenAddr;
        thePair.sym = tokenAddr.substr(0, 8);
        if (thePair.getPrice(bn, priceSource, price))
            return price;
    }

    priceSource = "not-priced";
    return 0.;
}

//---------------------------------------------------------------------------
double getPriceMaker_UsdPerEth(string_q& priceSource, blknum_t bn) {
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
    if (isTestMode()) {
        cerr << "TEST[DATE|TIME] ===================== maker pricing ========================" << endl;
        cerr << "TEST[DATE|TIME] CEthCall:" << endl;
        cerr << "TEST[DATE|TIME] blockNumber: " << theCall.blockNumber << endl;
        cerr << "TEST[DATE|TIME] address:     " << theCall.address << endl;
        cerr << "TEST[DATE|TIME] signature:   " << theCall.signature << endl;
        cerr << "TEST[DATE|TIME] encoding:    " << theCall.encoding << endl;
        cerr << "TEST[DATE|TIME] bytes:       " << theCall.bytes << endl;
    }
    if (doEthCall(theCall, true /* proxy */)) {
        CStringArray results;
        theCall.getCallResult(results);
        if (isTestMode()) {
            cerr << "TEST[DATE|TIME] result:" << endl;
            cerr << "TEST[DATE|TIME] " << results.size() << " results" << endl;
        }
        if (results.size() > 1 && results[1] == "true") {
            string_q hex = results[0];
            wei_t wei1 = str_2_Wei(hex);
            wei_t wei = wei1 * 100;
            wei_t ether = wei / weiPerEther();
            priceSource = "maker";
            double price = str_2_Uint(wei_2_Str(ether)) / 100.;
            if (isTestMode()) {
                cerr << "TEST[DATE|TIME] results[0]: " << results[0] << endl;
                cerr << "TEST[DATE|TIME] results[1]: " << results[1] << endl;
                cerr << "TEST[DATE|TIME] hex: " << hex << endl;
                cerr << "TEST[DATE|TIME] wei1: " << wei1 << endl;
                cerr << "TEST[DATE|TIME] wei: " << wei << endl;
                cerr << "TEST[DATE|TIME] weiPerEther: " << weiPerEther() << endl;
                cerr << "TEST[DATE|TIME] ether: " << ether << endl;
                cerr << "TEST[DATE|TIME] price: " << price << endl;
            }
            return price;
        } else if (isTestMode()) {
            LOG_INFO("doEthCall into makerMedianizer returned false");
        }
    } else {
        if (isTestMode()) {
            LOG_INFO("doEthCall in getPriceMaker_UsdPerEth returned false");
        }
    }

    priceSource = "not-priced";
    return 0.;
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
