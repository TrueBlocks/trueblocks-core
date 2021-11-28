## Example: simple

This example shows how to link an application to the TrueBlocks library (see the instructions below for installing the libraries)

The example then queries the blockchain for a blocks and prints each block as it's extracted to the console.

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
