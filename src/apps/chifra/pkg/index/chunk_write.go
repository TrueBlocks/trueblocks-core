package index

import (
	"encoding/binary"
	"io"
	"os"
	"path/filepath"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index/bloom"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/crypto"
)

type AddressAppearanceMap map[string][]AppearanceRecord

func WriteChunk(chain, fileName string, addrAppearanceMap AddressAppearanceMap, nApps int) error {
	// We're going to build two tables. An addressTable and an appearanceTable. We do this as we spin
	// through the map

	// Create space for the two tables...
	addressTable := make([]AddressRecord, 0, len(addrAppearanceMap))
	appearanceTable := make([]AppearanceRecord, 0, nApps)

	// We want to sort the items in the map by address (maps in GoLang are not sorted)
	sorted := []string{}
	for address := range addrAppearanceMap {
		sorted = append(sorted, address)
	}
	sort.Slice(sorted, func(i, j int) bool {
		return sorted[i] < sorted[j]
	})

	// We need somewhere to store our progress...
	offset := uint32(0)
	bl := bloom.ChunkBloom{}

	// For each address in the sorted list...
	for _, addrStr := range sorted {
		// ...get its appeances and append them to the appearanceTable....
		apps := addrAppearanceMap[addrStr]
		appearanceTable = append(appearanceTable, apps...)

		// ...add the address to the bloom filter...
		address := common.HexToAddress(addrStr)
		bl.AddToSet(address)

		// ...and append the record to the addressTable.
		addressTable = append(addressTable, AddressRecord{
			Address: address,
			Offset:  offset,
			Count:   uint32(len(apps)),
		})

		// Finally, note the next offset
		offset += uint32(len(apps))
	}

	// At this point, the two tables and the bloom filter are fully populated. We're ready to write to disc...

	// First, we backup the existing chunk if there is one...
	indexFn := ToIndexPath(fileName)
	tmpPath := filepath.Join(config.GetPathToCache(chain), "tmp")
	if backupFn, err := file.MakeBackup(tmpPath, indexFn); err == nil {
		defer func() {
			if file.FileExists(backupFn) {
				logger.Log(logger.Info, colors.BrightBlue, "Recovering from failure", backupFn, colors.Off)
				// If the backup file exists, something failed, so we replace the original file.
				os.Rename(backupFn, indexFn)
				os.Remove(backupFn) // seems redundant, but may not be on some operating systems
			}
		}()

		var fp *os.File
		if fp, err = os.OpenFile(indexFn, os.O_WRONLY|os.O_CREATE, 0644); err == nil {
			defer fp.Close() // defers are last in, first out

			fp.Seek(0, io.SeekStart) // already true, but can't hurt
			header := HeaderRecord{
				Magic:           file.MagicNumber,
				Hash:            common.BytesToHash(crypto.Keccak256([]byte(version.ManifestVersion))),
				AddressCount:    uint32(len(addressTable)),
				AppearanceCount: uint32(len(appearanceTable)),
			}
			if err = binary.Write(fp, binary.LittleEndian, header); err != nil {
				return err
			}

			if binary.Write(fp, binary.LittleEndian, addressTable); err != nil {
				return err
			}

			if binary.Write(fp, binary.LittleEndian, appearanceTable); err != nil {
				return err
			}

			if _, err = bl.WriteBloom(chain, bloom.ToBloomPath(indexFn)); err != nil {
				return err
			}

			// Success. Remove the backup so it doesn't replace the orignal
			os.Remove(backupFn)
			return nil
		}
	} else {
		return err
	}

	return nil
}

type Renderer interface {
	RenderObject(data interface{}, first bool) error
}
