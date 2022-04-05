package index

import (
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
)

const (
	// HeaderWidth - size of Header Record
	HeaderWidth = 44
)

// IndexData is one part of the two part IndexChunk (the other part is the BloomFilter)
//
// Each IndexData contains a HeaderRecord followed by two tables: the AddressTable and a related AppearanceTable.
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
type IndexData struct {
	File           *os.File
	Header         HeaderRecord
	Range          cache.FileRange
	AddrTableStart int64
	AppTableStart  int64
}

// NewIndexData returns an IndexData with an opened file pointer to the given fileName. The HeaderRecord
// for the chunk has been populated and the file position to the two tables are ready for use.
func NewIndexData(path string) (chunk IndexData, err error) {
	indexPath := toIndexPath(path)
	fmt.Println(indexPath)

	blkRange, err := cache.RangeFromFilename(indexPath)
	if err != nil {
		return IndexData{}, err
	}

	file, err := os.Open(indexPath)
	if err != nil {
		return IndexData{}, err
	}
	// Note, we don't defer closing here since we want the file to stay opened. Caller must close it.

	header, err := readHeader(file)
	if err != nil {
		file.Close()
		return IndexData{}, err
	}

	chunk = IndexData{
		File:           file,
		Header:         header,
		AddrTableStart: HeaderWidth,
		AppTableStart:  int64(HeaderWidth + (header.AddressCount * AddrRecordWidth)),
		Range:          blkRange,
	}

	return
}

// Close closes the IndexData's associated File pointer (if opened)
func (chunk *IndexData) Close() error {
	if chunk.File != nil {
		chunk.File.Close()
	}
	return nil
}
