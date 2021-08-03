/*-----------------------------------------------------------------------
 * qblocks - fast, fully-decentralized data from blockchains
 *----------------------------------------------------------------------*/
#include "acctlib.h"

//-----------------------------------------------------------------------
bool header(CTraverser* trav, void* data) {
    cout << "bn,tx_id,hash" << endl;
    return true;
}

//-----------------------------------------------------------------------
bool display(CTraverser* trav, void* data) {
    cout << trav->app->blk << ",";
    cout << trav->app->txid << ",";
    cout << trav->trans.hash << endl;
    return true;
}

//-----------------------------------------------------------------------
// This function creates a new traverser and returns it to the caller.
extern "C" CTraverser* makeTraverser(void) {
    CTraverser* trav = new CTraverser("testing");

    // Attach custom functions to the traverser.

    // preFunc is called before any transactions are encountered.
    trav->preFunc = header;

    // displayFunc is called at each transaction.
    trav->displayFunc = display;

    // The caller must clean up this memory.
    return trav;
}
