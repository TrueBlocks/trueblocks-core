package index

import (
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/ranges"
)

const (
	// HeaderWidth - size of Header Record
	HeaderWidth = 44
)

// Index is one part of the two part Chunk (the other part is the Bloom)
//
// Each Index contains a HeaderRecord followed by two tables: the AddressTable and a related AppearanceTable.
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
type Index struct {
	File           *os.File
	Header         indexHeader
	Range          ranges.FileRange
	AddrTableStart int64
	AppTableStart  int64
}

// OpenIndex returns an Index with an opened file pointer to the given fileName. The HeaderRecord
// for the chunk has been populated and the file position to the two tables are ready for use.
func OpenIndex(fileName string, check bool) (Index, error) {
	fileName = ToIndexPath(fileName)

	blkRange, err := ranges.RangeFromFilenameE(fileName)
	if err != nil {
		return Index{}, err
	}

	indexChunk := Index{
		AddrTableStart: HeaderWidth,
		Range:          blkRange,
	}
	indexChunk.File, err = os.OpenFile(fileName, os.O_RDONLY, 0)
	if err != nil {
		return Index{}, err
	}
	// Note, we don't defer closing here since we want the file to stay opened. Caller must close it.
	// defer idx.File.Close()

	indexChunk.Header, err = indexChunk.readHeader(check)
	if err != nil {
		indexChunk.Close()
		return Index{}, fmt.Errorf("%w: %s", err, fileName)
	}

	indexChunk.AppTableStart = int64(HeaderWidth + (indexChunk.Header.AddressCount * AddrRecordWidth))
	return indexChunk, nil
}

// Close closes the Index's associated File pointer (if opened)
func (chunk *Index) Close() error {
	if chunk.File != nil {
		chunk.File.Close()
		chunk.File = nil
	}
	return nil
}
