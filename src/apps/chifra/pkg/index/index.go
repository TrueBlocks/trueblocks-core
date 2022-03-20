package index

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"io"
	"log"
	"os"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/blockRange"
	"github.com/ethereum/go-ethereum/common"
)

const (
	// MagicNumber is used to check data validity
	MagicNumber = 0xdeadbeef
	// HeaderWidth - size of Header Record
	HeaderWidth = 44
	// AddrRecordWidth - size of Address Record
	AddrRecordWidth = 28
	// AppRecordWidth - size of Appearance Record
	AppRecordWidth = 8
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
	AddrTableStart int64
	AppTableStart  int64
	Range          blockRange.FileRange
}

// LoadIndexChunk returns an IndexChunk with an opened file pointer to the given fileName. The HeaderRecord
// for the chunk has been populated and the file position to the two tables are ready for use.
func LoadIndexChunk(fileName string) (IndexChunk, error) {
	blkRange, err := blockRange.RangeFromFilename(fileName)
	if err != nil {
		return IndexChunk{}, err
	}

	file, err := os.Open(fileName)
	if err != nil {
		return IndexChunk{}, err
	}

	header, err := readHeader(file)
	if err != nil {
		return IndexChunk{}, err
	}

	chunk := IndexChunk{
		File:           file,
		Header:         header,
		AddrTableStart: HeaderWidth,
		AppTableStart:  int64(HeaderWidth + (header.AddressCount * AddrRecordWidth)),
		Range:          blkRange,
	}

	return chunk, nil
}

// Close closes the IndexChunk's associated File pointer (if opened)
func (chunk *IndexChunk) Close() error {
	if chunk.File != nil {
		chunk.File.Close()
	}
	return nil
}

// ResultRecord carries the appearances found in a single IndexChunk for the given address.
type ResultRecord struct {
	Address    common.Address
	AppRecords *[]AppearanceRecord
}

// GetAppearanceRecords searches an already-opened IndexChunk for the given address. Returns a ResultRecord or nil
func (chunk *IndexChunk) GetAppearanceRecords(address common.Address) *ResultRecord {
	foundAt := chunk.searchForAddressRecord(address)
	if foundAt == -1 {
		return nil
	}

	startOfAddressRecord := int64(HeaderWidth + (foundAt * AddrRecordWidth))
	_, err := chunk.File.Seek(startOfAddressRecord, io.SeekStart)
	if err != nil {
		log.Println(err)
		return nil
	}

	addressRecord, err := chunk.readAddressRecord()
	if err != nil {
		log.Println(err)
		return nil
	}

	appearances, err := chunk.readAppearanceRecords(&addressRecord)
	if err != nil {
		log.Println(err)
		return nil
	}

	return &ResultRecord{Address: address, AppRecords: &appearances}
}

// HeaderRecord is the first 44 bytes of an IndexChunk file. This structure carries a magic number (4 bytes),
// a version specifier (32 bytes), and two four-byte integers representing the number of records in each
// of the two tables.
type HeaderRecord struct {
	Magic           uint32
	Hash            common.Hash
	AddressCount    uint32
	AppearanceCount uint32
}

func readHeader(file *os.File) (header HeaderRecord, err error) {
	err = binary.Read(file, binary.LittleEndian, &header)
	if err != nil {
		return
	}

	if header.Magic != MagicNumber {
		return header, fmt.Errorf("magic number in file %s is incorrect, expected %d, got %d", file.Name(), MagicNumber, header.Magic)
	}

	return
}

// AddressRecord is a single record in the Address table
type AddressRecord struct {
	Address common.Address
	Offset  uint32
	Count   uint32
}

func (chunk *IndexChunk) readAddressRecord() (addressRec AddressRecord, err error) {
	err = binary.Read(chunk.File, binary.LittleEndian, &addressRec)
	return
}

func (chunk *IndexChunk) searchForAddressRecord(address common.Address) int {
	compareFunc := func(pos int) bool {
		if pos == -1 {
			return false
		}

		if pos == int(chunk.Header.AddressCount) {
			return true
		}

		readLocation := int64(HeaderWidth + pos*AddrRecordWidth)
		_, err := chunk.File.Seek(readLocation, io.SeekStart)
		if err != nil {
			fmt.Println(err)
			return false
		}

		addressRec, err := chunk.readAddressRecord()
		if err != nil {
			fmt.Println(err)
			return false
		}

		return bytes.Compare(addressRec.Address[:], address[:]) >= 0
	}

	pos := sort.Search(int(chunk.Header.AddressCount), compareFunc)

	readLocation := int64(HeaderWidth + pos*AddrRecordWidth)
	chunk.File.Seek(readLocation, io.SeekStart)
	rec, err := chunk.readAddressRecord()
	if err != nil {
		return -1
	}

	if bytes.Compare(rec.Address[:], address[:]) != 0 {
		return -1
	}

	return pos
}

// AppearanceRecord is a single record in the Appearance table
type AppearanceRecord struct {
	BlockNumber   uint32 `json:"blockNumber"`
	TransactionId uint32 `json:"transactionIndex"`
}

func (chunk *IndexChunk) readAppearanceRecords(addrRecord *AddressRecord) (apps []AppearanceRecord, err error) {
	readLocation := int64(HeaderWidth + AddrRecordWidth*chunk.Header.AddressCount + AppRecordWidth*addrRecord.Offset)

	_, err = chunk.File.Seek(readLocation, io.SeekStart)
	if err != nil {
		return
	}

	apps = make([]AppearanceRecord, addrRecord.Count)
	err = binary.Read(chunk.File, binary.LittleEndian, &apps)

	return
}
