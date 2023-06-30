package index

import (
	"encoding/binary"
	"io"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
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

// TODO: hack alert - this is because of a circular dependency only. Could not import index into cache package
func AppsToNumPairs(apps []AppearanceRecord) []base.NumPair[uint32] {
	pairs := make([]base.NumPair[uint32], len(apps))
	for index, app := range apps {
		pairs[index] = base.NumPair[uint32]{N1: app.BlockNumber, N2: app.TransactionId}
	}
	return pairs
}

func (chunk *ChunkData) ReadAppearanceRecordsAndResetOffset(addrRecord *AddressRecord) (apps []AppearanceRecord, err error) {
	offset, err := chunk.File.Seek(0, io.SeekCurrent)
	if err != nil {
		return apps, err
	}

	apps, err = chunk.ReadAppearanceRecords(addrRecord)
	if err != nil {
		return apps, err
	}

	_, err = chunk.File.Seek(offset, io.SeekStart)
	if err != nil {
		return apps, err
	}

	return apps, nil
}

func (chunk *ChunkData) ReadAppearanceRecords(addrRecord *AddressRecord) (apps []AppearanceRecord, err error) {
	readLocation := int64(HeaderWidth + AddrRecordWidth*chunk.Header.AddressCount + AppRecordWidth*addrRecord.Offset)

	_, err = chunk.File.Seek(readLocation, io.SeekStart)
	if err != nil {
		return
	}

	apps = make([]AppearanceRecord, addrRecord.Count)
	err = binary.Read(chunk.File, binary.LittleEndian, &apps)

	return
}

func (app *AppearanceRecord) ReadAppearance(file *os.File) (err error) {
	return binary.Read(file, binary.LittleEndian, app)
}
