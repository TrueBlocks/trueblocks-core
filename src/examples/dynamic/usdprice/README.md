## Dynamic Traverser : usdPrice

An example of a [Dynamic Traverser](https://docs.trueblocks.io/blog/dynamic-traversers-for-trueblocks/) that shows the price of ETH to USD at every transaction for a given address. It uses the UniSwap contract as a price oracle. It does not price the transaciton, but it could.

### Running

```[shell]
chifra export --load libusdprice.so <address>
```

### Building

See the instructions in the file [./src/examples/README.md](../../README.md) for more information.
