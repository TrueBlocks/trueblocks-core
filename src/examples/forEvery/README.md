## Example: forEvery

This example show how to use a few of TrueBlocks' `forEvery` functions.

TrueBlocks provides various `forEvery` functions which do pretty much exactly what they sound like they do. They apply an arbitrary function to a data structure, much like Javascript's `map` function.

Below is a list of all available `forEvery` routines at the time of this writing.

### Building

See the instructions in the file [./src/examples/README.md](../README.md) for more information.

### ForEvery Functions

#### Top level

- on Blocks
  - bool `forEveryBlock`(BLOCKVISITFUNC func, void* data, uint64_t start, uint64_t count, uint64_t skip)
  - bool `forEveryBlock`(BLOCKVISITFUNC func, void* data, const string_q& block_list)
- on Transactions
  - bool `forEveryTransaction`(TRANSVISITFUNC func, void* data, const string_q& trans_list)
- on Traversers
  - bool `forEveryAppearance`(const CTraverserArray& traversers, const CAppearanceArray_base& apps, void* data)

#### Against C++ classes

- CRuntimeClass
  - bool `forEveryField`(FIELDVISITFUNC func, void* data)
- COptionsBlockList
  - bool `forEveryBlockNumber`(UINT64VISITFUNC func, void* data) const
- CBlock
  - bool `forEveryTransaction`(TRANSVISITFUNC func, void* data)
  - bool `forEveryLog`(LOGVISITFUNC func, void* data)
  - bool `forEveryTrace`(TRACEVISITFUNC func, void* data)
- CTransaction
  - bool `forEveryLog`(LOGVISITFUNC func, void* data)
  - bool `forEveryTrace`(TRACEVISITFUNC func, void* data)

#### Misecelaneous

- against ascii files
  - bool `forEveryLineInAsciiFile`(const string_q& filenameIn, CHARPTRFUNC func, void* data)
- against folders
  - bool `forEveryFileInFolder`(const string_q& mask, CONSTAPPLYFUNC func, void* data)

#### Other things

- bool `COperation::forEveryExtraction`(APPLYFUNC func, void *data)
- bool `COperation::forEveryDownload`(APPLYFUNC func, void *data)
- bool `COperation::forEveryDerivation`(APPLYFUNC func, void *data)
- bool `COperation::forEveryCombine`(APPLYFUNC func, void *data)
- bool `COperation::forEveryOperation`(APPLYFUNC func, void *data)
