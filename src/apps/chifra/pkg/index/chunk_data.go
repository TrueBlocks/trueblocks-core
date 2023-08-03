package index

import (
	"encoding/json"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

const (
	// HeaderWidth - size of Header Record
	HeaderWidth = 44
)

// ChunkData is one part of the two part Chunk (the other part is the ChunkBloom)
//
// Each ChunkData contains a HeaderRecord followed by two tables: the AddressTable and a related AppearanceTable.
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
type ChunkData struct {
	File           *os.File
	Header         IndexHeaderRecord
	Range          base.FileRange
	AddrTableStart int64
	AppTableStart  int64
}

// NewChunkData returns an ChunkData with an opened file pointer to the given fileName. The HeaderRecord
// for the chunk has been populated and the file position to the two tables are ready for use.
func NewChunkData(path string) (chunk ChunkData, err error) {
	indexPath := ToIndexPath(path)

	blkRange, err := base.RangeFromFilenameE(indexPath)
	if err != nil {
		return ChunkData{}, err
	}

	file, err := os.OpenFile(indexPath, os.O_RDONLY, 0)
	if err != nil {
		return ChunkData{}, err
	}
	// Note, we don't defer closing here since we want the file to stay opened. Caller must close it.

	header, err := readIndexHeader(file)
	if err != nil {
		file.Close()
		return ChunkData{}, err
	}

	chunk = ChunkData{
		File:           file,
		Header:         header,
		AddrTableStart: HeaderWidth,
		AppTableStart:  int64(HeaderWidth + (header.AddressCount * AddrRecordWidth)),
		Range:          blkRange,
	}

	return
}

// String returns a JSON representation of the Chunk
func (chunk ChunkData) String() string {
	s, _ := json.MarshalIndent(chunk, "", "  ")
	return string(s)
}

// Close closes the ChunkData's associated File pointer (if opened)
func (chunk *ChunkData) Close() error {
	if chunk.File != nil {
		chunk.File.Close()
		chunk.File = nil
	}
	return nil
}
