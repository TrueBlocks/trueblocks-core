## Dynamic Traverser : gasHole

This dynamic traverser is simalar to [./sample1](../sample1/README.md), but it does a little bit more work.

At each transaction, it queries the amount of gas spent on the transaction if the transaction was sent by the account we're
traversing. The traverser keeps track of the total gas spent over time as well as how much of that expenditure was due to
in-error transactions. This is not possible with an indexer that relies solely on event data as no events are generated 
for in-error tansactions.

This is one of the lovely things about the traverser idea. It gives the developer complete freedom on how each transaction
is handled all the way down to the very bottom of the data. You need not work within the framework of any external system.

To run this traverser, first build it (see below) and then run this command using any address (or addresses):

```
chifra export --load gasHole <address> [address...]
```

In the code, you will notice how we're created a custom traverser...

```
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
```

...and we return that custom traverser when we're initialized...

```
extern "C" CTraverser* makeTraverser(void) {
    CTestTraverser* trav = new CTestTraverser;
    trav->preFunc = header;
    trav->displayFunc = display;
    return trav;
}
```

...Also notice that the traverser is carried through as each appearance of the address is visited...

```
//-----------------------------------------------------------------------
bool display(CTraverser* trav, void* data) {
    CTestTraverser* tt = (CTestTraverser*)trav;
    ....
```

...and that, not only do we calculate at each transaction how much gas was spent, but we accumulate those numbers as we go...

```
    wei_t spent = tt->trans.gasPrice * tt->trans.receipt.gasUsed;
    tt->totalSpent += spent;
    tt->totalError += (tt->trans.isError ? spent : 0);
```

...and finally, we present the result:

```
    string_q val = wei_2_Ether(spent, 18);
    string_q tot = wei_2_Ether(tt->totalSpent, 18);
    string_q err = wei_2_Ether(tt->totalError, 18);

    cout << trav->app->blk << "," << trav->app->txid << "," << val << "," << tot << "," << err << endl;
```

In the last line of the `Display` function, we return `true` to tell the traverser to continue. If we return `false` the traversal stops.

### Building

See the instructions in the file [./src/examples/README.md](../../README.md) for more information.
