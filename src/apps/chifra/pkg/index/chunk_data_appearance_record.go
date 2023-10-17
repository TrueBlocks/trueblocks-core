package index

import (
	"encoding/binary"
	"io"
	"os"
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

func (chunk *ChunkIndex) ReadAppearanceRecordsAndResetOffset(addrRecord *AddressRecord) (apps []AppearanceRecord, err error) {
	offset, err := chunk.File1.Seek(0, io.SeekCurrent)
	if err != nil {
		return apps, err
	}

	apps, err = chunk.ReadAppearanceRecords(addrRecord)
	if err != nil {
		return apps, err
	}

	_, err = chunk.File1.Seek(offset, io.SeekStart)
	if err != nil {
		return apps, err
	}

	return apps, nil
}

func (chunk *ChunkIndex) ReadAppearanceRecords(addrRecord *AddressRecord) (apps []AppearanceRecord, err error) {
	readLocation := int64(HeaderWidth + AddrRecordWidth*chunk.Header.AddressCount + AppRecordWidth*addrRecord.Offset)

	_, err = chunk.File1.Seek(readLocation, io.SeekStart)
	if err != nil {
		return
	}

	apps = make([]AppearanceRecord, addrRecord.Count)
	err = binary.Read(chunk.File1, binary.LittleEndian, &apps)

	return
}

func (app *AppearanceRecord) ReadAppearance(file *os.File) (err error) {
	return binary.Read(file, binary.LittleEndian, app)
}
