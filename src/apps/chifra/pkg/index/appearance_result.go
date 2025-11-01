package index

import (
	"encoding/binary"
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/ranges"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
)

// AppearanceResult carries the appearances found in a single Index for the given address.
type AppearanceResult struct {
	Address    base.Address
	Range      ranges.FileRange
	AppRecords *[]types.AppRecord
	Err        error
}

// ReadAppearances searches an already-opened Index for the given address. Returns a AppearanceResult or nil
func (chunk *Index) ReadAppearances(address base.Address) *AppearanceResult {
	ret := AppearanceResult{Address: address, Range: chunk.Range}

	foundAt := chunk.searchForAddressRecord(address)
	if foundAt == -1 {
		return &ret
	}

	startOfAddressRecord := int64(HeaderWidth + (foundAt * AddrRecordWidth))
	_, err := chunk.File.Seek(startOfAddressRecord, io.SeekStart)
	if err != nil {
		ret.Err = err
		return &ret
	}

	addressRecord := types.AddrRecord{}
	if err := binary.Read(chunk.File, binary.LittleEndian, &addressRecord); err != nil {
		ret.Err = err
		return &ret
	}

	appearances, err := chunk.readAppearanceRecords(&addressRecord)
	if err != nil {
		ret.Err = err
		return &ret
	}

	ret.AppRecords = &appearances
	return &ret
}
