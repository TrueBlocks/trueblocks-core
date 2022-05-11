## Dynamic Traverser : simple1

This dynamic traverser scans an account's history and presents each transaction that the given account appears in. For each of
those transactions, the traverser shows the `blockNuber`, `transactionIndex`, and `transactionHash`.

Notice how simple this is from the perspective of the programmer. The following code works against any Ethereum address no matter how
large or how small.

To run this traverser, first build it (see below) and then run this command using any address (or addresses):

```
chifra export --load simple1 <address> [address...]
```

**Note:** The extracted blockchain data is not stored locally. This allows the tool to work against addresses with 10's or 100's of transactions as easily as those with 100's of millions of transactions. The end user, however, may choose to cache this data by sending the `--cache` option. Be careful, though, the data extraction can get quite large.

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
extern "C" CTraverser* makeTraverser(uint32_t wanted) {
    CTraverser* trav = new CTraverser("testing");

    trav->preFunc = header;
    trav->displayFunc = display;

    return trav;
}
```

### Building

See the instructions in the file [./src/examples/README.md](../../README.md) for more information.
