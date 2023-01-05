# Four-bytes database generator

```bash
make
./bin/four_bytes --outdir /where/to/put/chunks
```

It generates the cross-product database of four-bytes from the data already present in TrueBlocks.
The input files are installed with `chifra`:
- `abis/known-000/uniq_sigs.tab` (Set A)
- `abis/known-000/uniq_funcs.tab` (Set B)
- `abis/known-000/uniq_events.tab` (Set B)

Set A has 784 items.

Set B has 113,281 items in total.

Taking the cross-product (784 x 113,281) of these two sets produces 88,812,304 four-byte signatures, which is about 88 times bigger than the four-byte directory. It's a vanishingly small precentage of the 2^32 data set of four-byte encodings, though.

The produced database is chunked and consists of 65,536 files, with sizes ranging from 100 KB to 176 KB.
The database takes about 7 GB of disk space in total. Generation can take up to 40 minutes.

## A Second Set of Sets A and B

We have a second set B with more than 616,287 function and events names and a second set A with 41,531 signatures. The cross produce of these two sets would produce about 25,595,015,397 results. Since this is larger than the space that needs to be covered (2^32 == 4,294,967,296) it would cover the entire space with an eight times over redundancy. Someone could produce this database, publish it on some content addressable store (such as IPFS -- it is immutable after all and perfectly reproducible). Over time, people might go to that IPFS hash to find four-bytes. We could build a TCR (Hi Auryn!) to sort out which of the 25 redundant functions are the right ones.

## Chunk format

Chunk format tries to follow Unchained Index file format as close as possible. Each chunk consist of 3 parts:
```
[      HEADER      ]
[ Signature Table  ]
[   String Table   ]
```

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

1. Jay:
    > This might speed things up very significantly and not lose very much data. A while ago, I looked at about 850,000 functions (I forget where I got them). I extracted the type signatures. There were about 45,000 uniq type signatures, but the attached 500 accounted for 99% of the entries.

2. Cross-product database has many entries that has been never seen on chain. Maybe we could add frequency information to give priority to the *real* signatures
