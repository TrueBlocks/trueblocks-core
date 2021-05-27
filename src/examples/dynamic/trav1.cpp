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
    trav->os << trav->app->blk << ",";
    trav->os << trav->app->txid << ",";
    trav->os << trav->trans.hash << endl;
    return true;
}

//-----------------------------------------------------------------------
// This function creates a new traverser and returns it to the caller.
extern "C" CTraverser *makeTraverser(void) {
    CTraverser* trav = new CTraverser(cout, "testing");

    // We attach functions to a traverser. Here we set most functions
    // to the noopFunc which simply returns true.
    trav->filterFunc = trav->postFunc = trav->dataFunc = noopFunc;

    // preFunc is called before any transactions are encountered.
    trav->preFunc = header;

    // displayFunc is called at each transaction.
    trav->displayFunc = display;

    // The caller must clean up this memory.
    return trav;
}





