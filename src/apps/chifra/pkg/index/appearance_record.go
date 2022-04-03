package index

import (
	"encoding/binary"
	"io"
	"log"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/ethereum/go-ethereum/common"
)

const (
	// AppRecordWidth - size of Appearance Record
	AppRecordWidth = 8
)

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

// ResultRecord carries the appearances found in a single IndexChunk for the given address.
type ResultRecord struct {
	Address    common.Address
	Range      cache.FileRange
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

	return &ResultRecord{Address: address, Range: chunk.Range, AppRecords: &appearances}
}
