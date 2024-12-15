# Four-bytes database generator

```bash
make
./bin/four_bytes --outdir /where/to/put/chunks
```

It generates the cross-product database of four-bytes signatures from a small collection of function names and type signatures.

## Sets A and B

The input files are installed with `chifra` and appear here:

- `./src/other/install/abis/known-000/uniq_sigs.tab` (Set A)
- `./src/other/install/abis/known-000/uniq_funcs.tab` (Set B)
- `./src/other/install/abis/known-000/uniq_events.tab` (Set B)

Set A has 784 items. Set B has 113,281 items in total. Taking the cross-product of those two sets (784 x 113,281) produces 88,812,304 four-byte signatures. This is about 88 times bigger than the four-byte directory, but it is a vanishingly small precentage of the 2^32 (4,294,967,296) possible four byte encodings.

The database produced from these sets is "chunked" by the first two bytes in the four-byte. Similar to the Unchained Index. This produces 65,536 files. Files can be downloaded by end users from IPFS and pinned, making the file increasingly available to others.

The size of the files ranges between 100  and 200 KB. The full database takes about 7 GB of disk space in total, but most users would store much less data than that. Generating the database takes about 40 minutes.

## A Second Set of Sets

We have another set of sets. In this other set, Set A has 41,531 signatures. Set B has 616,287 function and events names.

The cross product of these two sets produces 25,595,015,397 results. This second cross product is eight times larger than it needs to be. Eight signatures for each of the four billion four-byte encodings. We could filter out many of the less frequently used functions and signature types producing much smaller data.

In some future world, someone could produce this database, publish it (in a chunked manner) to a content-addressable store (such as IPFS), and over time, people would learn to go to this well-known IPFS hash to find four-bytes encodings. The more people use it, the better it would get.

We could even build a TCR (hi Auryn!) to help us sort out which of the 25 redundant encodings is the right one.

A simple linear extrapolation of the above sizes suggests about it would take about 288 times longer or about eight days. (But it would be done once and for all.)

## Chunk format

The chunks are stored in a binary file with the following format. The chunk format follows Unchained Index file format as closely as possible. Each chunk consist of 3 parts:
```
[ Header ]
[ Signature Table ]
[ String Table ]
```

where,

### Header

```go
type Header struct {
	Magic          uint32
	Hash           common.Hash
	SignatureCount uint32
}
```

### Signature Table (could be called Encoding Table)

an array of

```go
type SignatureRecord struct {
	Signature common.Hash // Where the string representation starts in the file?
	Offset    uint32      // How long is the string representation?
	Len       uint32      // The length of the string found at that location
}
```

### String Table (could be called Signature Table)

A binary blob of strings packed tightly together, thus:

`islets()tixFounders()reclaimToken(address)partisans(uint256)setPurchaseValues(uint128)`

These strings are indexed by the Offet and Len found in the Signature Table.

## An Algorithm for Searching for Fourbytes

It is assumed that the above files, of which there may be many depending on the number of bytes used to chunk the database, are memory mapped. Upon opening a particular file, the location and length of the Signature Table is known and can be binary searched for the four-byte being queried. Depending on the application and the amount of available memory, the file may remain open for future queries. Upon locating a four-byte, the Offset and Len may be used to retrieve the Function (or Event) signature string.

Note: We do not store the full 32-byte encodings for events. If searching for events, one can search for the first four bytes of the event encoding which will return either one or many signatures. One can easily regenerate the full 32-byte encoding from these strings, thereby identifying the event encoding. In this way, the size of the Signature Table (which should be called the Encoding Table) can be kept smaller.

## Further improvements

- We may be able to speed things up using a second database (or actually a "first" database) where we store well-known encodings. (as an obvious example, the enoding for Transfer.) This would speed things up significantly and produce identical results. Some time ago, we studied nearly 600,000 function names along with 45,000 type signatures. About 500 of the combinations accounted for more than 90% of the actual four-byte and event encodings found on chain.

- The cross-product databases naturally has many entries that have never actually appeared on the chain. It should be possible to scan the entire history of the chain and build record the frequency of both four-bytes and event topic0's. When subsequently searching for en encoding, we could sort the resulting cross product by frequency. Searching 25,000,000,000 signatures, 24,999,999,250 of which have never appeared on-chain, as we do now, seems a bit counter productive.
