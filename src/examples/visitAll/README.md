## visitAll example

This example show how to use a few of TrueBlocks' `forEvery` functions.

`forEvery` functions sort of do what they sound like they would do. The apply an arbitrary function to a data structure, much like Javascripts `map` function. Below is a list of all available `forEvery` routines at the time of this writing. Please check the docs for more information.

### Top level

- on Blocks
  - bool `forEveryBlock`(BLOCKVISITFUNC func, void* data, uint64_t start, uint64_t count, uint64_t skip)
  - bool `forEveryBlock`(BLOCKVISITFUNC func, void* data, const string_q& block_list)
  - bool `forEveryBlock_light`(BLOCKVISITFUNC func, void* data, uint64_t start, uint64_t count, uint64_t skip)
- on Transactions
  - bool `forEveryTransaction`(TRANSVISITFUNC func, void* data, const string_q& trans_list)
- on Logs
  - bool `forEveryLogInBlock`(LOGVISITFUNC func, void* data, const CBlock& block)
  - bool `forEveryLogInTransaction`(LOGVISITFUNC func, void* data, const CTransaction& trans)
- on Traces
  - bool `forEveryTraceInBlock`(TRACEVISITFUNC func, void* data, const CBlock& block)
  - bool `forEveryTraceInTransaction`(TRACEVISITFUNC func, void* data, const CTransaction& trans)
- on Index data
  - bool `forEveryIndexChunk`(INDEXCHUNKFUNC func, void* data)
  - bool `forEveryIndexBloom`(INDEXBLOOMFUNC func, void* data)
  - bool `forEveryAddressInIndex`(ADDRESSFUNC func, const blkrange_t& range, void* data)
  - bool `forEverySmartContractInIndex`(ADDRESSFUNC func, void* data)
- on Traversers
  - bool `forEveryAppearance`(const CTraverserArray& traversers, const CAppearanceArray_base& apps, void* data)

### Against c++ classes

- CRuntimeClass
  - bool `forEveryField`(FIELDVISITFUNC func, void* data)
- COptionsBlockList
  - bool `forEveryBlockNumber`(UINT64VISITFUNC func, void* data) const
- COptionsBase
  - bool `forEverySpecialBlock`(NAMEVALFUNC func, void* data)
  - bool `forEveryNamedAccount`(NAMEFUNC func, void* data)
- CBlock
  - bool `forEveryTransaction`(TRANSVISITFUNC func, void* data)
  - bool `forEveryUniqueAppearanceInBlock`(APPEARANCEFUNC func, TRANSFUNC filter, void* data);
  - bool `forEveryUniqueAppearanceInBlockPerTx`(APPEARANCEFUNC func, TRANSFUNC filter, void* data);
  - bool `forEveryAppearanceInBlock`(APPEARANCEFUNC func, TRANSFUNC filter, void* data);
- CTransaction
  - bool `forEveryAppearanceInTx`(APPEARANCEFUNC func, TRANSFUNC filter, void* data);
  - bool `forEveryUniqueAppearanceInTx`(APPEARANCEFUNC func, TRANSFUNC filter, void* data);
  - bool `forEveryUniqueAppearanceInTxPerTx`(APPEARANCEFUNC func, TRANSFUNC filter, void* data);

### Misecelaneous

- against ascii files
  - bool forEveryLineInAsciiFile(const string_q& filenameIn, CHARPTRFUNC func, void* data)
- against folders
  - bool forEveryFileInFolder(const string_q& mask, FILEVISITOR func, void* data)
- against pinata pins
  - bool pinlib_forEveryPin(CPinnedChunkArray& pList, PINFUNC func, void* data)
- on Merkle trees
  - bool `forEveryAccount`(CTreeRoot* trie, ACCTVISITOR func, void* data)

### Other things

- bool COperation::forEveryExtraction(APPLYFUNC func, void *data)
- bool COperation::forEveryDownload(APPLYFUNC func, void *data)
- bool COperation::forEveryDerivation(APPLYFUNC func, void *data)
- bool COperation::forEveryCombine(APPLYFUNC func, void *data)
- bool COperation::forEveryOperation(APPLYFUNC func, void *data)
