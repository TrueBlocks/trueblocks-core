The TrueBlocks index scraper periodically creates a chunked portion of the index so that it can be more easily stored in a content-addresable data store such as IPFS. We call these periodically-created chunks, PinnedChunks. The format of said item is described here. A pinned chunk is effectively a relational table relating all of the addresses appearing in the chunk with a list of appearances appearing in the chunk.

The following commands produce and manage [{PLURAL}]:

| Tools                                              |                                                                 |
| -------------------------------------------------- | --------------------------------------------------------------- |
| [chifra scrape](/docs/chifra/admin/#chifra-scrape) | scan the chain and update the TrueBlocks index of appearances   |
| [chifra chunks](/docs/chifra/admin/#chifra-chunks) | manage pinned index of appearances and associated Bloom filters |

[{PROPER}] data is made of the following data fields:

[{FIELDS}]
