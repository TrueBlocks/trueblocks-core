/*-----------------------------------------------------------------------
 * qblocks - fast, fully-decentralized data from blockchains
 *----------------------------------------------------------------------*/
#include "acctlib.h"

//-----------------------------------------------------------------------
class CTestTraverser : public CTraverser {
  public:
    wei_t totalSpent;
    wei_t totalError;
    CTestTraverser(void) : CTraverser("testing") {
        totalSpent = 0;
        totalError = 0;
    }
};

//-----------------------------------------------------------------------
bool header(CTraverser* trav, void* data) {
    cout << "timestamp\tgasCostEther\ttotalSpent\ttotalErr" << endl;
    return true;
}

//-----------------------------------------------------------------------
bool display(CTraverser* trav, void* data) {
    CTestTraverser* tt = (CTestTraverser*)trav;

    wei_t spent = tt->trans.gasPrice * tt->trans.receipt.gasUsed;
    tt->totalSpent += spent;
    tt->totalError += (tt->trans.isError ? spent : 0);

    string_q val = wei_2_Ether(spent, 18);
    string_q tot = wei_2_Ether(tt->totalSpent, 18);
    string_q err = wei_2_Ether(tt->totalError, 18);

    cout << trav->app->blk << "," << trav->app->txid << "," << val << "," << tot << "," << err << endl;
    return true;
}

//-----------------------------------------------------------------------
extern "C" CTraverser* makeTraverser(void) {
    CTestTraverser* trav = new CTestTraverser;
    trav->preFunc = header;
    trav->displayFunc = display;
    return trav;
}
