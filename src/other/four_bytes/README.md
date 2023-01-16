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

Chunk format tries to follow Unchained Index file format as close as possible. Each chunk consist of 3 parts:
```
[ HEADER ]
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

### Signature Table

is array of
```go
type SignatureRecord struct {
	Signature common.Hash
	// Where the string representation starts in the file?
	Offset    uint32
	// How long is the string representation?
	Len       uint32
}
```

### String Table

is strings packed next to each other, e.g.:

`islets()tixFounders()reclaimToken(address)partisans(uint256)setPurchaseValues(uint128)`

## Further improvements

1. We might be able to speed things up with a second database (or actually a "first" database) where we store 'known' encodings. This might speed things up significantly and produce nearly the same results. Some time ago, we looked at about 600,000 functions and about 45,000 uniq type signatures. About 500 of them accounted for about 99% of the entries in the cross product.

2. The cross-product databases will naturally have many entries that ahve never actually shown up on the chain. We could scan the entire history of hte chain and build a database of the frequency of both four-bytes and topic0's. When searching for a four-byte, we could sort the cross product by frequency. Searching 25,000,000,000 signatures, 24,999,999,250 of which have never appeared on-chain seems a bit strange.
