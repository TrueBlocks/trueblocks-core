## Dynamic Traversers

The examples in this folder show how one builds and uses [Dynamic Traversers](https://docs.trueblocks.io/blog/dynamic-traversers-for-trueblocks/).

The idea behind a dynamic traverser is to allow the programmer to build a very minimal library that, given each transaction in an account's history, 
can do custom handling. Using the `chifra export --load <shared_object> <address>` command, each transaction in the given account's history
is sent to the shared object. 

The programmer need focus only on the particular task at hand, ignoring the traversal which is handled by `chifra export`.

| Name                                   | Description                                                                                                  |
| -------------------------------------- | ------------------------------------------------------------------------------------------------------------ |
| [simple1](./simple1/README.md)         | A simple example of a dynamic traverser                                                                      |
| [gasHole](./dynamic/gasHole/README.md) | A simple example show how to account for gas spent of sucessful and failed transactions                      |
| [balances](./balances/README.md)       | A simple example of producing a balance history for an account                                               |
| [filtering](./filtering/README.md)     | An example of filtering for particular data in an account's transaction history                              |
| [recons](./recons/README.md)           | An example of reconciling an account's history at every transaction                                          |
| [usdprice](./usdprice/README.md)       | An example of creating spot prices for an account after each transaction using the UniSwap V2 smart contract |

### Building

See the instructions in the file [./src/examples/README.md](../../README.md) for more information.
