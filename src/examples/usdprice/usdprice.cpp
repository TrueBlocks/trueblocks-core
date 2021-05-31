/*-----------------------------------------------------------------------
 * qblocks - fast, fully-decentralized data from blockchains
 *----------------------------------------------------------------------*/
#include "acctlib.h"


string_q uniswapFactory = "0x5c69bee701ef814a2b6a3edd4b1652cb9cc5aa6f";
string_q getPair = "0xe6a43905";
string_q pairBytes = "0000000000000000000000006b175474e89094c44da98b954eedeac495271d0f000000000000000000000000c02aaa39b223fe8d0a0e5c4f27ead9083c756cc2";
string_q getReserves = "0x0902f1ac";
string_q reserveBytes = "";
//-----------------------------------------------------------------------
class CTestTraverser : public CTraverser {
  public:
    CEthCall perTxCall;
    CTestTraverser(void) : CTraverser(cout, "testing") {
        CEthCall theCall;
        theCall.address = uniswapFactory;
        theCall.encoding = getPair;
        theCall.bytes = pairBytes;
        theCall.abi_spec.loadAbiFromEtherscan(uniswapFactory);
        theCall.blockNumber = getBlockProgress(BP_CLIENT).client;
        if (doEthCall(theCall)) {
            perTxCall.address = theCall.getResults();
            perTxCall.abi_spec.loadAbiFromEtherscan(perTxCall.address);
            cerr << "Found USD Pair: " << perTxCall.address << " with ";
            cerr << perTxCall.abi_spec.interfaces.size() << " endpoints" << endl;
        }
        perTxCall.encoding = getReserves;
        perTxCall.bytes = reserveBytes;
    }
};

//-----------------------------------------------------------------------
bool header(CTraverser* trav, void* data) {
    cout << "date\t";
    cout << "blockNumber\t";
    cout << "transactionId\t";
    cout << "reserve1\t";
    cout << "reserve2\t";
    cout << "priceUSD" << "\t";
    cout << "balEth" << "\t";
    cout << "balUSD" << endl;
    return true;
}

//-----------------------------------------------------------------------
bool display(CTraverser* trav, void* data) {
    CTestTraverser* tt = (CTestTraverser*)trav;
    tt->perTxCall.blockNumber = tt->app->blk;
    if (doEthCall(tt->perTxCall) && !tt->perTxCall.result.outputs.empty()) {
        cout << tt->trans.Format("[{DATE}]") << "\t";
        cout << tt->app->blk << "\t";
        cout << tt->app->txid << "\t";
        CStringArray results;
        if (tt->perTxCall.getResults(results) && results.size() > 1) {
            wei_t balance = getBalanceAt(tt->accountedFor, tt->app->blk);
            double reserve1 = str_2_Double(wei_2_Ether(str_2_Wei(results[0]), 18));
            double reserve2 = str_2_Double(wei_2_Ether(str_2_Wei(results[1]), 18));
            double ethbal = str_2_Double(wei_2_Ether(balance, 18));
            double price = reserve1 / reserve2;
            double usdbal = ethbal * price;
            cout << double_2_Str(reserve1, 3) << "\t";
            cout << double_2_Str(reserve2, 3) << "\t";
            cout << double_2_Str(price, 2) << "\t";
            cout << double_2_Str(ethbal, 3)  << "\t";
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
    
    freshenTimestamps(getBlockProgress().client);
    loadTimestamps(&expContext().tsMemMap, expContext().tsCnt);

    CTestTraverser* trav = new CTestTraverser;
    trav->filterFunc = trav->postFunc = trav->dataFunc = noopFunc;
    trav->preFunc = header;
    trav->displayFunc = display;

    return trav;
}
// 0xa478c2975ab1ea89e8196811f51a7b7ade33eb11
// on,both,fast,state,tools/getState,uniswap_factory_getPair,y,call =
// 0x5c69bee701ef814a2b6a3edd4b1652cb9cc5aa6f!0xe6a43905!0000000000000000000000002260fac5e5542a773aa44fbcfedf7c193bc2c599000000000000000000000000c02aaa39b223fe8d0a0e5c4f27ead9083c756cc2
// & blocks = 10092000 on,both,fast,state,tools/getState,uniswap_token0,y,call =
// 0xbb2b8038a1640196fbe3e38816f3e67cba72d940!0x0dfe1681 & blocks = 10092000
// on,both,fast,state,tools/getState,uniswap_token0_name,y,call = 0x2260fac5e5542a773aa44fbcfedf7c193bc2c599!0x06fdde03
// & blocks = 10092000 on,both,fast,state,tools/getState,uniswap_token1,y,call =
// 0xbb2b8038a1640196fbe3e38816f3e67cba72d940!0xd21220a7 & blocks = 10092000
// on,both,fast,state,tools/getState,uniswap_token1_name,y,call = 0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2!0x06fdde03
// & blocks = 10092000 on,both,fast,state,tools/getState,uniswap_name,y,call =
// 0xbb2b8038a1640196fbe3e38816f3e67cba72d940!0x06fdde03 & blocks = 10092000
// on,both,fast,state,tools/getState,uniswap_reserve,y,call = 0xbb2b8038a1640196fbe3e38816f3e67cba72d940!0x0902f1ac &
// blocks = 10092000 on,both,fast,state,tools/getState,uniswap_fail,y,call =
// 0xbb2b8038a1640196fbe3e38816f3e67cba72d940!0x0902f1ad & blocks = 10092000
