package index

import (
	"encoding/binary"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index/bloom"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinning"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
	"github.com/ethereum/go-ethereum/crypto"
)

type AddressAppearanceMap map[string][]AppearanceRecord
type AddressBooleanMap map[string]bool
type AppearanceMap map[string]types.SimpleAppearance
type WriteChunkReport struct {
	Range        base.FileRange
	nAddresses   int
	nAppearances int
	Snapped      bool
	Pinned       bool
	PinRecord    manifest.ChunkRecord
}

func (c *WriteChunkReport) Report() {
	str := fmt.Sprintf("%sWrote %d address and %d appearance records to $INDEX/%s.bin%s%s", colors.BrightBlue, c.nAddresses, c.nAppearances, c.Range, colors.Off, spaces20)
	if c.Snapped {
		str = fmt.Sprintf("%sWrote %d address and %d appearance records to $INDEX/%s.bin %s%s%s", colors.BrightBlue, c.nAddresses, c.nAppearances, c.Range, colors.Yellow, "(snapped to grid)", colors.Off)
	}
	logger.Info(str)
	if c.Pinned {
		str := fmt.Sprintf("%sPinned chunk $INDEX/%s.bin (%s,%s)%s", colors.BrightBlue, c.Range, c.PinRecord.IndexHash, c.PinRecord.BloomHash, colors.Off)
		logger.Info(str)
	}
}

func WriteChunk(chain, fileName string, publisher base.Address, addrAppearanceMap AddressAppearanceMap, nApps int, pin, remote bool) (*WriteChunkReport, error) {
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
		address := base.HexToAddress(addrStr)
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
				// If the backup file exists, something failed, so we replace the original file.
				_ = os.Rename(backupFn, indexFn)
				_ = os.Remove(backupFn) // seems redundant, but may not be on some operating systems
			}
		}()

		if fp, err := os.OpenFile(indexFn, os.O_WRONLY|os.O_CREATE, 0644); err == nil {
			// defer fp.Close() // Note -- we don't defer because we want to close the file and possibly pin it below...

			_, _ = fp.Seek(0, io.SeekStart) // already true, but can't hurt
			header := IndexHeaderRecord{
				Magic:           file.MagicNumber,
				Hash:            base.BytesToHash(crypto.Keccak256([]byte(version.ManifestVersion))),
				AddressCount:    uint32(len(addressTable)),
				AppearanceCount: uint32(len(appearanceTable)),
			}
			if err = binary.Write(fp, binary.LittleEndian, header); err != nil {
				return nil, err
			}

			if err = binary.Write(fp, binary.LittleEndian, addressTable); err != nil {
				return nil, err
			}

			if err = binary.Write(fp, binary.LittleEndian, appearanceTable); err != nil {
				return nil, err
			}

			if _, err = bl.WriteBloom(chain, ToBloomPath(indexFn)); err != nil {
				return nil, err
			}

			if err := fp.Sync(); err != nil {
				return nil, err
			}

			if err := fp.Close(); err != nil { // Close the file so we can pin it
				return nil, err
			}

			// We're sucessfully written the chunk, so we don't need this any more. If the pin
			// fails we don't want to have to re-do this chunk, so remove this here.
			os.Remove(backupFn)

			rng := base.RangeFromFilename(indexFn)
			report := WriteChunkReport{ // For use in reporting...
				Range:        rng,
				nAddresses:   len(addressTable),
				nAppearances: len(appearanceTable),
				Pinned:       pin,
			}

			if !pin {
				return &report, nil
			}

			result, err := pinning.PinChunk(chain, ToBloomPath(indexFn), ToIndexPath(indexFn), remote)
			if err != nil {
				return &report, err
			}

			path := config.GetPathToIndex(chain) + "ts.bin"
			if _, err = pinning.PinItem(chain, "timestamps", path, remote); err != nil {
				return &report, err
			}

			rec := ResultToRecord(&result)
			report.PinRecord.IndexHash = rec.IndexHash
			report.PinRecord.BloomHash = rec.BloomHash
			report.PinRecord.IndexSize = rec.IndexSize
			report.PinRecord.BloomSize = rec.BloomSize
			return &report, manifest.UpdateManifest(chain, publisher, rec)

		} else {
			return nil, err
		}

	} else {
		return nil, err
	}
}

var spaces20 = strings.Repeat(" ", 20)

func ResultToRecord(result *pinning.PinResult) manifest.ChunkRecord {
	if len(result.Local.BloomHash) > 0 {
		return manifest.ChunkRecord{
			Range:     result.Range.String(),
			IndexHash: result.Local.IndexHash,
			IndexSize: result.Local.IndexSize,
			BloomHash: result.Local.BloomHash,
			BloomSize: result.Local.BloomSize,
		}
	}
	return manifest.ChunkRecord{
		Range:     result.Range.String(),
		IndexHash: result.Remote.IndexHash,
		IndexSize: result.Remote.IndexSize,
		BloomHash: result.Remote.BloomHash,
		BloomSize: result.Remote.BloomSize,
	}
}
