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
#include "acctlib.h"

string_q uniswapFactory = "0x5c69bee701ef814a2b6a3edd4b1652cb9cc5aa6f";
string_q getPair = "0xe6a43905";
string_q pairBytes = padLeft("6b175474e89094c44da98b954eedeac495271d0f", 64, '0') +
                     padLeft("c02aaa39b223fe8d0a0e5c4f27ead9083c756cc2", 64, '0');
string_q getReserves = "0x0902f1ac";
string_q reserveBytes = "";
//-----------------------------------------------------------------------
class CTestTraverser : public CTraverser {
  public:
    CEthCall perTxCall;
    CTestTraverser(void) : CTraverser("testing") {
        CEthCall theCall;
        theCall.address = uniswapFactory;
        theCall.encoding = getPair;
        theCall.bytes = pairBytes;
        theCall.abi_spec.loadAbiFromEtherscan(uniswapFactory);
        theCall.blockNumber = getBlockProgress(BP_CLIENT).client;
        LOG4("Calling ", substitute(theCall.Format(), "\n", " "));
        if (doEthCall(theCall)) {
            perTxCall.address = theCall.getCallResult();
            perTxCall.abi_spec.loadAbiFromEtherscan(perTxCall.address);
            LOG_INFO(bGreen, "Found USD Pair: ", perTxCall.address, " with ", perTxCall.abi_spec.nInterfaces(),
                     " endpoints", cOff);
        } else {
            LOG_WARN(bRed, "Could not find USD Pair: ", perTxCall.address, cOff);
        }
        perTxCall.encoding = getReserves;
        perTxCall.bytes = reserveBytes;
    }
};

//-----------------------------------------------------------------------
bool header(CTraverser* trav, void* data) {
    cout << "hash\t";
    cout << "date\t";
    cout << "bn.txid\t";
    cout << "reserveDAI\t";
    cout << "reserveETH\t";
    cout << "priceUSD\t";
    cout << "balEth\t";
    cout << "balUSD" << endl;
    return true;
}

//-----------------------------------------------------------------------
bool display(CTraverser* trav, void* data) {
    CTestTraverser* tt = (CTestTraverser*)trav;
    tt->perTxCall.blockNumber = tt->app->blk;
    tt->trans.timestamp = getTimestampAt(tt->app->blk);
    tt->block.timestamp = getTimestampAt(tt->app->blk);

    cerr << tt->searchType << " ";
    if (doEthCall(tt->perTxCall) && !tt->perTxCall.callResult.outputs.empty()) {
        CStringArray results;
        if (tt->perTxCall.getCallResult(results) && results.size() > 1) {
            wei_t balance = getBalanceAt(tt->curMonitor->address, tt->app->blk);
            double reserve1 = str_2_Double(wei_2_Ether(str_2_Wei(results[0]), 18));
            double reserve2 = str_2_Double(wei_2_Ether(str_2_Wei(results[1]), 18));
            double ethbal = str_2_Double(wei_2_Ether(balance, 18));
            double price = reserve1 / reserve2;
            double usdbal = ethbal * price;

            cout << tt->trans.hash << "\t";
            cout << tt->trans.Format("[{DATE}]") << "\t";
            cout << tt->app->blk << "." << tt->app->txid << "\t";
            cout << double_2_Str(reserve1, 2) << "\t";
            cout << double_2_Str(reserve2, 2) << "\t";
            cout << double_2_Str(price, 2) << "\t";
            cout << double_2_Str(ethbal, 3) << "\t";
            cout << double_2_Str(usdbal, 2) << endl;
        }

    } else {
        cerr << "No result at block " << tt->app->blk << "\r";
        cerr.flush();
    }

    return true;
}

//-----------------------------------------------------------------------
extern "C" CTraverser* makeTraverser(void) {
    acctlib_init(quickQuitHandler);
    if (getVersionNum() < getVersionNum(0, 18, 0)) {
        LOG_ERR("Cannot load traverser from older versions: ", getVersionNum());
        LOG_ERR("Perhaps you need to re-install TrueBlocks.");
        return nullptr;
    }

    CTestTraverser* trav = new CTestTraverser;
    trav->preFunc = header;
    trav->dataFunc = noopFunc;
    trav->displayFunc = display;

    return trav;
}
