/*-----------------------------------------------------------------------
 * qblocks - fast, fully-decentralized data from blockchains
 *----------------------------------------------------------------------*/
#include "acctlib.h"

//-----------------------------------------------------------------------
class CTestTraverser : public CTraverser {
  public:
    wei_t totalSpent;
    wei_t totalError;
    CTestTraverser(void) : CTraverser(cout, "testing") {
        totalSpent = 0;
        totalError = 0;
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

    wei_t balance = getBalanceAt(tt->accountedFor, tt->app->blk);

    cout << trav->trans.Format("[{DATE}]") << ",";
    cout << wei_2_Ether(balance, 18) << ",";
    cout << wei_2_Dollars(trav->trans.timestamp, balance, 18) << endl;
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
