## Dynamic Traverser : filtering

This example describes Dynamic Traversers which are [documented in this blog post](). This traverses scans an account's history and presents all of the transaction hashes that this address has appeared in.

```
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
extern "C" CTraverser* makeTraverser(void) {
    CTraverser* trav = new CTraverser("testing");

    trav->preFunc = header;
    trav->displayFunc = display;

    return trav;
}
```

### Building

See the instructions in the file [./src/examples/README.md](../../README.md) for more information.
