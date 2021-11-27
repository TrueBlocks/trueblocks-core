## Example: findFirst

This example starts at a given block and scans forward through the chain until it encounters the block where
"The DAO" first appears anywhere in the block (i.e. where it was deployed).

```
bool visitAddress(const CAppearance& item, void* data) {
    if (item.addr == *reinterpret_cast<address_t*>(data)) {
        cout << "\nFound address at: " << item;
        return false;
    }

    cerr << item.addr << " ";
    cerr << padNum9(item.bn) << " ";
    cerr << padNum5(item.tx) << " ";
    cerr << item.reason << "                \r";
    cerr.flush();

    return true;
}

//-----------------------------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);

    blknum_t startBlock = 1428700;

    address_t theDaoAddr("0xbb9bc244d798123fde783fcc1c72d3bb8c189413");

    for (blknum_t bl = startBlock; bl < getBlockProgress(BP_CLIENT).client; bl++) {
        CBlock block;
        getBlock(block, bl);
        if (!block.forEveryUniqueAppearanceInTxs(visitAddress /* func */, NULL /* filterFunc */, &theDaoAddr /* data */))
            return 0;
    }

    etherlib_cleanup();

    return 0;
}
```

### Building

See the instructions in the file [./src/examples/README.md](../README.md) for more information.
