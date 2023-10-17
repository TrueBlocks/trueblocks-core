package index

import (
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// AppearanceResult carries the appearances found in a single ChunkIndex for the given address.
type AppearanceResult struct {
	Address    base.Address
	Range      base.FileRange
	AppRecords *[]AppearanceRecord
	Err        error
}

// GetAppearanceRecords searches an already-opened ChunkIndex for the given address. Returns a AppearanceResult or nil
func (chunk *ChunkIndex) GetAppearanceRecords(address base.Address) *AppearanceResult {
	ret := AppearanceResult{Address: address, Range: chunk.Range}

	foundAt := chunk.searchForAddressRecord(address)
	if foundAt == -1 {
		return &ret
	}

	startOfAddressRecord := int64(HeaderWidth + (foundAt * AddrRecordWidth))
	_, err := chunk.File1.Seek(startOfAddressRecord, io.SeekStart)
	if err != nil {
		ret.Err = err
		return &ret
	}

	addressRecord := AddressRecord{}
	err = addressRecord.ReadAddress(chunk.File1)
	if err != nil {
		ret.Err = err
		return &ret
	}

	appearances, err := chunk.ReadAppearanceRecords(&addressRecord)
	if err != nil {
		ret.Err = err
		return &ret
	}

	ret.AppRecords = &appearances
	return &ret
}
