## Example: simple

This example shows how to link to the TrueBlocks library (after installing it) and then queries the blockchain for a block and prints the block to the screen.

```
int main(int argc, const char* argv[]) {

    etherlib_init(quickQuitHandler);

    CBlock block;
    getBlock(block, 3500000);

    cout << block << endl;

    return 0;
}
```

### Building

See the instructions in the file [./src/examples/README.md](../README.md) for more information.
