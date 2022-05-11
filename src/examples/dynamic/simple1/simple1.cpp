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
extern "C" CTraverser* makeTraverser(uint32_t wanted) {
    if (getVersionNum() < wanted) {
        LOG_ERR("Cannot load traverser from older versions: ", getVersionNum());
        LOG_ERR("You need to re-install TrueBlocks and rebuild this traverser.");
        return nullptr;
    }

    CTraverser* trav = new CTraverser("testing");

    trav->preFunc = header;
    trav->displayFunc = display;

    return trav;
}
