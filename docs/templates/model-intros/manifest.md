The Manifest details the index of appearance's PinnedChunks. Each record in the Manifest details the block range represented by the chunk as well as the IPFS hash of the index chunk along with the associated IPFS hash for the Bloom filter of the chunk. The manifest itself is also pushed to IPFS and the IPFS of the hash of the manifest is published periodically to the Unchained Index smart contract.

The following commands produce and manage [{PLURAL}]:

| Tools                                              |                                                                 |
| -------------------------------------------------- | --------------------------------------------------------------- |
| [chifra scrape](/docs/chifra/admin/#chifra-scrape) | scan the chain and update the TrueBlocks index of appearances   |
| [chifra chunks](/docs/chifra/admin/#chifra-chunks) | manage pinned index of appearances and associated Bloom filters |

[{PROPER}] data is made of the following data fields:

[{FIELDS}]
