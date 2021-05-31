/*-----------------------------------------------------------------------
 * qblocks - fast, fully-decentralized data from blockchains
 *----------------------------------------------------------------------*/
#include "acctlib.h"

//-----------------------------------------------------------------------
class CTestTraverser : public CTraverser {
  public:
    CEthCall perTxCall;
    CTestTraverser(void) : CTraverser(cout, "testing") {
        CEthCall theCall;
        theCall.address = "0x5c69bee701ef814a2b6a3edd4b1652cb9cc5aa6f";
        theCall.encoding = "0xe6a43905";
        theCall.bytes =
            "0000000000000000000000006b175474e89094c44da98b954eedeac495271d0f000000000000000000000000c02aaa39b223fe8d0a"
            "0e5c4f27ead9083c756cc2";
        theCall.blockNumber = getBlockProgress(BP_CLIENT).client;
        theCall.abi_spec.loadAbiFromEtherscan(theCall.address);
        if (doEthCall(theCall)) {
            theCall.getResults(perTxCall.address);
            perTxCall.abi_spec.loadAbiFromEtherscan(perTxCall.address);
            cerr << "USD Pair: " << perTxCall.address << ": " << perTxCall.abi_spec.interfaces.size() << " endpoints"
                 << endl;
        }
        freshenTimestamps(getBlockProgress().client);
        loadTimestamps(&expContext().tsMemMap, expContext().tsCnt);
        CEthCall::registerClass();
        CFunction::registerClass();
        perTxCall.encoding = "0x0902f1ac";
        perTxCall.bytes = "";
    }
};

//-----------------------------------------------------------------------
bool header(CTraverser* trav, void* data) {
    cout << "date,";
    cout << "balanceEth,";
    cout << "balanceUSD" << endl;
    return true;
}

//-----------------------------------------------------------------------
bool display(CTraverser* trav, void* data) {
    CTestTraverser* tt = (CTestTraverser*)trav;
    tt->perTxCall.blockNumber = tt->app->blk;
    if (doEthCall(tt->perTxCall) && !tt->perTxCall.result.outputs.empty()) {
        cout << tt->trans.Format("[{DATE}]") << "\t";
        // cout << wei_2_Ether(balance, 18) << ",";
        // cout << wei_2_Dollars(trav->trans.timestamp, balance, 18) << endl;
        CStringArray results;
        if (tt->perTxCall.getResults(results)) {
            if (results.size() > 1) {
                double reserve1 = str_2_Double(wei_2_Ether(str_2_Wei(results[0]), 18));
                double reserve2 = str_2_Double(wei_2_Ether(str_2_Wei(results[1]), 18));
                double price = reserve1 / reserve2;
                cout << double_2_Str(reserve1, 3) << "\t" << double_2_Str(reserve2, 3) << "\t" << double_2_Str(price, 2)
                     << endl;
            }
        }
    } else {
        cerr << "No result at block " << tt->app->blk << "\r";
        cerr.flush();
    }

    return true;
}

//-----------------------------------------------------------------------
extern "C" CTraverser* makeTraverser(void) {
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
