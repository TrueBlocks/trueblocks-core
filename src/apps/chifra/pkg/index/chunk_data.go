package index

import (
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

const (
	// HeaderWidth - size of Header Record
	HeaderWidth = 44
)

// ChunkIndex is one part of the two part Chunk (the other part is the ChunkBloom)
//
// Each ChunkIndex contains a HeaderRecord followed by two tables: the AddressTable and a related AppearanceTable.
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
type ChunkIndex struct {
	File1          *os.File
	Header         IndexHeader
	Range          base.FileRange
	AddrTableStart int64
	AppTableStart  int64
}

// NewChunkIndex returns an ChunkIndex with an opened file pointer to the given fileName. The HeaderRecord
// for the chunk has been populated and the file position to the two tables are ready for use.
func NewChunkIndex(path string) (chunk ChunkIndex, err error) {
	indexPath := ToIndexPath(path)

	blkRange, err := base.RangeFromFilenameE(indexPath)
	if err != nil {
		return ChunkIndex{}, err
	}

	fp, err := os.OpenFile(indexPath, os.O_RDONLY, 0)
	if err != nil {
		return ChunkIndex{}, err
	}
	// Note, we don't defer closing here since we want the file to stay opened. Caller must close it.

	header, err := X_ReadIndexHeader(fp, config.GetUnchained().HeaderMagic, false /* unused */)
	if err != nil {
		fp.Close()
		return ChunkIndex{}, fmt.Errorf("%w: %s", err, indexPath)
	}

	chunk = ChunkIndex{
		File1:          fp,
		Header:         header,
		AddrTableStart: HeaderWidth,
		AppTableStart:  int64(HeaderWidth + (header.AddressCount * AddrRecordWidth)),
		Range:          blkRange,
	}

	return
}

// Close closes the ChunkIndex's associated File pointer (if opened)
func (chunk *ChunkIndex) Close() error {
	if chunk.File1 != nil {
		chunk.File1.Close()
		chunk.File1 = nil
	}
	return nil
}
