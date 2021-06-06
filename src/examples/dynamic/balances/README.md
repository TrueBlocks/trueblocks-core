### Balances Dynamic Traverser

A quick example of a [Dynamic Traverser](https://docs.trueblocks.io/blog/dynamic-traversers-for-trueblocks/) to extract balance histories for a given address.

### Building

**Note:** This only builds on Linux. We'd love to have a more platform independant makefile. Easy first issue.

```[shell]
cd ./trueblocks-core/src/examples/balances
make
```

This places a shared object, called `libbalances.so` into your cache at `$CACHE/objs/libbalances.so`

### Running

```[shell]
chifra export --load libbalances.so <address>
```
