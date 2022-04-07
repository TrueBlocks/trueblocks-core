package index

import (
	"io"
	"log"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/ethereum/go-ethereum/common"
)

// AppearanceResult carries the appearances found in a single ChunkData for the given address.
type AppearanceResult struct {
	Address    common.Address
	Range      cache.FileRange
	AppRecords *[]AppearanceRecord
}

// GetAppearanceRecords searches an already-opened ChunkData for the given address. Returns a AppearanceResult or nil
func (chunk *ChunkData) GetAppearanceRecords(address common.Address) *AppearanceResult {
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

	return &AppearanceResult{Address: address, Range: chunk.Range, AppRecords: &appearances}
}
