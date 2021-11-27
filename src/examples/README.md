## Examples

The `examples` folder contains a small collection of example code that may help illustrate how to write your own programs using TrueBlocks. This is a community generated repository. If you wish to add to this folder, feel free to do so by making a pull request.

### Folders

The examples directly in the folder detail how to use TrueBlocks to write something like a command line tool. Other great sources for examples of command line tools are in the ./src/tools folder.

In the folder, `dynamic` are a few examples of building [custom traversers](https://docs.trueblocks.io/blog/dynamic-traversers-for-trueblocks/). See the README in that folder for more information.

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