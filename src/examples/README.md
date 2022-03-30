## Examples

The `examples` folder contains a small collection of example code that may help illustrate how to write your own programs using TrueBlocks. This is a community generated repository. If you wish to add to this folder, feel free to do so by making a pull request.

### Building

In order to build the examples in this folder, you must have first run `make install` from the `./build` folder. If you experience trouble building these examples, first make sure to do `make install`.


### Folders

The examples directly in the folder detail how to use TrueBlocks to write something like a command line tool. Other great sources for examples of command line tools are in the ./src/tools folder.

| Name                               | Description                                                                          |
| ---------------------------------- | ------------------------------------------------------------------------------------ |
| [simple](./simple/README.md)       | A very simple example                                                                |
| [findFirst](./findFirst/README.md) | An example that scans the chain looking for the deployment block of a smart contract |
| [forEvery](./forEvery/README.md)   | An example of using the `forEvery` routines                                          |
| [txCounter](./txCounter/README.md) | A more complicated example counting the number of blocks per week                    |


In the folder, `dynamic` are a few examples of building [custom traversers](https://docs.trueblocks.io/blog/dynamic-traversers-for-trueblocks/). See the README in that folder for more information.

| Name                                       | Description                                                                                                  |
| ------------------------------------------ | ------------------------------------------------------------------------------------------------------------ |
| [simple1](./dynamic/simple1/README.md)     | A simple example of a dynamic traverser                                                                      |
| [gasHole](./dynamic/gasHole/README.md)     | A simple example show how to account for gas spent of sucessful and failed transactions                      |
| [balances](./dynamic/balances/README.md)   | A simple example of producing a balance history for an account                                               |
| [filtering](./dynamic/filtering/README.md) | An example of filtering for particular data in an account's transaction history                              |
| [recons](./dynamic/recons/README.md)       | An example of reconciling an account's history at every transaction                                          |
| [usdprice](./dynamic/usdprice/README.md)   | An example of creating spot prices for an account after each transaction using the UniSwap V2 smart contract |

### Building

Before proceeding, you must install the TrueBlocks headers and static libraries in a location where the make files can find them. (That is, `/usr/local/include/tb` and `/usr/local/lib/tb` respectively.)

Do this with from the `./build` folder:

```
make -j
make install
```

To build each example program, enter into that folder and type

```
make
```

The results of the build will appear in the `./bin` folder in the present working directory.
