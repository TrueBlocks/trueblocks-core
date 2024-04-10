package index

import (
	"encoding/binary"
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

const (
	// AppRecordWidth - size of Appearance Record
	AppRecordWidth = 8
)

func (chunk *Index) ReadAppearancesAndReset(addrRecord *types.SimpleAddrRecord) (apps []types.SimpleAppRecord, err error) {
	offset, err := chunk.File.Seek(0, io.SeekCurrent)
	if err != nil {
		return apps, err
	}

	apps, err = chunk.readAppearanceRecords(addrRecord)
	if err != nil {
		return apps, err
	}

	_, err = chunk.File.Seek(offset, io.SeekStart)
	if err != nil {
		return apps, err
	}

	return apps, nil
}

func (chunk *Index) readAppearanceRecords(addrRecord *types.SimpleAddrRecord) (apps []types.SimpleAppRecord, err error) {
	readLocation := int64(HeaderWidth + AddrRecordWidth*chunk.Header.AddressCount + AppRecordWidth*addrRecord.Offset)

	_, err = chunk.File.Seek(readLocation, io.SeekStart)
	if err != nil {
		return
	}

	apps = make([]types.SimpleAppRecord, addrRecord.Count)
	err = binary.Read(chunk.File, binary.LittleEndian, &apps)

	return
}
