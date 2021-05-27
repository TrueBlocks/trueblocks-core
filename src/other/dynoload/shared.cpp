#include "acctlib.h"

class CTestTraverser : public CTraverser {
  public:
    wei_t totalSpent;
    wei_t totalError;
    CTestTraverser(void) : CTraverser(cout, "testing") {
        totalSpent = 0;
        totalError = 0;
    }
};

bool pre(CTraverser* trav, void* data) {
    cout << "timestamp\tgasCostEther\ttotalSpent\ttotalErr" << endl;
    return true;
}

bool disp(CTraverser* trav, void* data) {
    CTestTraverser *tt = (CTestTraverser*)trav;
    // cout << "out: " << tt << endl;
    // if (tt->trans.isError) {
        tt->totalSpent += (tt->trans.gasPrice * tt->trans.receipt.gasUsed);
        tt->totalError += (trav->trans.isError ? (trav->trans.gasPrice * trav->trans.receipt.gasUsed) : 0);
        string_q val = wei_2_Ether(trav->trans.gasPrice * trav->trans.receipt.gasUsed, 18);
        string_q tot = wei_2_Ether(tt->totalSpent, 18);
        string_q err = wei_2_Ether(tt->totalError, 18);
        trav->os << (trav->index++) << "\t" << trav->app->blk << "\t" << trav->app->txid << "\t" << trav->trans.timestamp << "\t" << val << "\t" << tot << "\t" << err << endl;
    // }
    return true;
}

extern "C" CTraverser *makeTraverser(void) {
    CTestTraverser* trav = new CTestTraverser;
    // cout << "in: " << trav << endl;
    trav->filterFunc = trav->preFunc = trav->postFunc = trav->dataFunc = noopFunc;
    trav->preFunc = pre;
    trav->displayFunc = disp;
    return trav;
}
