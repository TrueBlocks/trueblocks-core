<!-- markdownlint-disable MD033 MD036 MD041 -->
The Manifest details the index of appearance's PinnedChunks. Each record in the Manifest details
the block range represented by the chunk as well as the IPFS hash of the index chunk along with
the associated IPFS hash for the Bloom filter of the chunk. The manifest itself is also pushed
to IPFS and the IPFS of the hash of the manifest is published periodically to the Unchained Index
smart contract.
