package index

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
)

const (
	// HeaderWidth - size of Header Record
	HeaderWidth = 44
)

// IndexChunk represents a consolidated portion of the Index of All Appearances (called a Chunk). The name
// of the file is the form Start-End.bin (where both Start and End are nine character, zero-left-padded block
// numbers representing the first block and the last block (inclusive) of the range covered by this chunk.
//
// Each IndexChunk contains a HeaderRecord followed by two tables: the AddressTable and a related AppearanceTable.
//
// The HeaderRecord (44 bytes long) contains a four-byte magic number (`0xdeadbeef` -- to indicate we're reading
// a file of the correct type), a 32-byte hash representing the file's version, and two 4-byte integers representing
// the number of records in the AddressTable (nAddresses) and the number of records in the AppearanceTable (nAppearances)
// respectively.
//
// The AddressTable has nAddresses records, one for each address that appears in any block between Start to End
// inclusive. These addresses are found using the TrueBlocks Appearance Finder algorithm. Each AddressRecord consists
// of a 20-byte address followed by two 4-byte integers representing the Offset into the AppearanceTable where
// this address's list of appearances starts followed by the Count of appearance records for the given address.
//
// The AppearanceTable contains nAppeeances pairs of <blockNumber.transactionId> pairs arranged by the Offset
// and Count pairs found in the corresponding AddressTable records.
type IndexChunk struct {
	File           *os.File
	Header         HeaderRecord
	Range          cache.FileRange
	AddrTableStart int64
	AppTableStart  int64
}

// NewIndexChunk returns an IndexChunk with an opened file pointer to the given fileName. The HeaderRecord
// for the chunk has been populated and the file position to the two tables are ready for use.
func NewIndexChunk(fileName string) (chunk IndexChunk, err error) {
	blkRange, err := cache.RangeFromFilename(fileName)
	if err != nil {
		return IndexChunk{}, err
	}

	file, err := os.Open(fileName)
	if err != nil {
		return IndexChunk{}, err
	}
	// Note, we don't defer closing here since we want the file to stay opened. Caller has to close it.

	header, err := readHeader(file)
	if err != nil {
		file.Close()
		return IndexChunk{}, err
	}

	chunk = IndexChunk{
		File:           file,
		Header:         header,
		AddrTableStart: HeaderWidth,
		AppTableStart:  int64(HeaderWidth + (header.AddressCount * AddrRecordWidth)),
		Range:          blkRange,
	}

	return
}

// Close closes the IndexChunk's associated File pointer (if opened)
func (chunk *IndexChunk) Close() error {
	if chunk.File != nil {
		chunk.File.Close()
	}
	return nil
}
