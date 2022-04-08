package index

import (
	"encoding/binary"
	"io"
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

func (chunk *ChunkData) readAppearanceRecords(addrRecord *AddressRecord) (apps []AppearanceRecord, err error) {
	readLocation := int64(HeaderWidth + AddrRecordWidth*chunk.Header.AddressCount + AppRecordWidth*addrRecord.Offset)

	_, err = chunk.File.Seek(readLocation, io.SeekStart)
	if err != nil {
		return
	}

	apps = make([]AppearanceRecord, addrRecord.Count)
	err = binary.Read(chunk.File, binary.LittleEndian, &apps)

	return
}
