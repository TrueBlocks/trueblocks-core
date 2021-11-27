## Dynamic Traverser : recons

An example of a [Dynamic Traverser](https://docs.trueblocks.io/blog/dynamic-traversers-for-trueblocks/) that shows the price of ETH to USD at every transaction for a given address. It uses the UniSwap contract as a price oracle. It does not price the transaciton, but it could.

### Building

**Note:** This only builds on Linux. We'd love to have a more platform independant makefile. Easy first issue.

```[shell]
cd ./trueblocks-core/src/examples/usdprice
make
```

This places a shared object, called `libusdprice.so` into your cache at `$CACHE/objs/libusdprice.so`

### Running

```[shell]
chifra export --load libusdprice.so <address>
```

### Building

See the instructions in the file [../../README.md](./src/examples/README.md) for more information.

### Building

See the instructions in the file [./src/examples/README.md](../../README.md) for more information.
