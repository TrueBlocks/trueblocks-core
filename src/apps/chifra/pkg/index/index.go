package index

import (
	"encoding/binary"
	"fmt"
	"io"
	"os"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/address"
)

const (
	// HashLength = number of bytes in a HASH
	HashLength = 32
	// MagicNumber is used to check data validity
	MagicNumber = 0xdeadbeef
	// HeaderWidth - size of Header Record
	HeaderWidth = 44
	// AddrRecordWidth - size of Address Record
	AddrRecordWidth = 28
	// AppRecordWidth - size of Appearance Record
	AppRecordWidth = 8
)

type EthHash [HashLength]byte

// TODO BOGUS -- LOT'S LEFT TO DO HERE
type IndexChunk struct {
	File           *os.File
	Header         HeaderRecord
	AddrTableStart int64
	AppTableStart  int64
}

func NewIndexChunk(file *os.File, header HeaderRecord) IndexChunk {
	return IndexChunk{
		File:           file,
		Header:         header,
		AddrTableStart: HeaderWidth,
		AppTableStart:  int64(HeaderWidth + (header.AddressCount * AddrRecordWidth)),
	}
}

func LoadIndexChunk(path string) (IndexChunk, error) {
	file, err := os.Open(path)
	if err != nil {
		return IndexChunk{}, err
	}

	header, err := ReadHeader(file)
	if err != nil {
		return IndexChunk{}, err
	}

	chunk := NewIndexChunk(file, header)
	return chunk, nil
}

// HeaderRecord is the header record in the file. Carries sizes for the other tables (total 44 bytes)
//    - Magic (4 bytes)
//    - Hash (32 bytes)
//    - AddressCount (4 bytes)
//    - AppearanceCount (4 bytes)
type HeaderRecord struct {
	Magic           uint32
	Hash            EthHash
	AddressCount    uint32
	AppearanceCount uint32
}

func ReadHeader(file *os.File) (header HeaderRecord, err error) {
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
	Address address.EthAddress
	Offset  uint32
	Count   uint32
}

func ReadAddressRecord(f *os.File) (addressRec AddressRecord, err error) {
	err = binary.Read(f, binary.LittleEndian, &addressRec)
	return
}

// AppearanceRecord is a single record in the Appearance table
type AppearanceRecord struct {
	BlockNumber   uint32 `json:"blockNumber"`
	TransactionId uint32 `json:"transactionIndex"`
}

func ReadAppearanceRecords(f *os.File, numAddresses uint32, addrRecord *AddressRecord) (apps []AppearanceRecord, err error) {
	readLocation := int64(HeaderWidth + AddrRecordWidth*numAddresses + AppRecordWidth*addrRecord.Offset)

	_, err = f.Seek(readLocation, io.SeekStart)
	if err != nil {
		return
	}

	apps = make([]AppearanceRecord, addrRecord.Count)
	err = binary.Read(f, binary.LittleEndian, &apps)

	return
}

func SearchForAddressRecord(f *os.File, numRecs uint32, addr address.EthAddress) int {
	compareFunc := func(pos int) bool {
		if pos == -1 {
			return false
		}

		if pos == int(numRecs) {
			return true
		}

		readLocation := int64(HeaderWidth + pos*AddrRecordWidth)
		_, err := f.Seek(readLocation, io.SeekStart)
		if err != nil {
			fmt.Println(err)
			return false
		}

		addressRec, err := ReadAddressRecord(f)
		if err != nil {
			fmt.Println(err)
			return false
		}

		return addressRec.Address.Compare(addr) >= 0
	}

	pos := sort.Search(int(numRecs), compareFunc)

	readLocation := int64(HeaderWidth + pos*AddrRecordWidth)
	f.Seek(readLocation, io.SeekStart)
	rec, err := ReadAddressRecord(f)
	if err != nil {
		return -1
	}

	if rec.Address.Compare(addr) != 0 {
		return -1
	}

	return pos
}
