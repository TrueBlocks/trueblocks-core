package index

import (
	"encoding/binary"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
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

// TODO: BOGUS - PINNING TO PINATA AND WRITING MANIFEST FILE
func WriteChunk(chain, fileName string, addAppMap AddressAppearanceMap, nApps, snapper int) (bool, error) {
	addressTable := make([]AddressRecord, 0, len(addAppMap))
	appearanceTable := make([]AppearanceRecord, 0, nApps)
	sorted := []string{}
	for address := range addAppMap {
		sorted = append(sorted, address)
	}
	sort.Slice(sorted, func(i, j int) bool {
		return sorted[i] < sorted[j]
	})

	bl := bloom.ChunkBloom{}

	offset := uint32(0)
	for _, addrStr := range sorted {
		apps := addAppMap[addrStr]
		appearanceTable = append(appearanceTable, apps...)
		address := common.HexToAddress(addrStr)
		bl.AddToSet(address)
		addressTable = append(addressTable, AddressRecord{
			Address: address,
			Offset:  offset,
			Count:   uint32(len(apps)),
		})
		offset += uint32(len(apps))
	}

	var err error

	fileName = ToIndexPath(fileName)
	tmpPath := filepath.Join(config.GetPathToCache(chain), "tmp")
	// Make a backup copy of the file in case the write fails so we can replace it...
	if backupFn, err := file.MakeBackup(fileName, tmpPath); err == nil {
		defer func() {
			if file.FileExists(backupFn) {
				logger.Log(logger.Info, colors.BrightBlue, "Recovering from failure", backupFn, colors.Off)
				// If the backup file exists, something failed, so we replace the original file.
				os.Rename(backupFn, fileName)
				os.Remove(backupFn) // seems redundant, but may not be on some operating systems
			}
		}()

		var fp *os.File
		if fp, err = os.OpenFile(fileName, os.O_WRONLY|os.O_CREATE, 0644); err == nil {
			defer fp.Close() // defers are last in, first out

			fp.Seek(0, io.SeekStart) // already true, but can't hurt
			header := HeaderRecord{
				Magic:           file.MagicNumber,
				Hash:            common.BytesToHash(crypto.Keccak256([]byte(version.ManifestVersion))),
				AddressCount:    uint32(len(addressTable)),
				AppearanceCount: uint32(len(appearanceTable)),
			}
			if err = binary.Write(fp, binary.LittleEndian, header); err != nil {
				return false, err
			}

			if binary.Write(fp, binary.LittleEndian, addressTable); err != nil {
				return false, err
			}

			if binary.Write(fp, binary.LittleEndian, appearanceTable); err != nil {
				return false, err
			}

			if _, err = bl.WriteBloom(chain, bloom.ToBloomPath(fileName)); err != nil {
				return false, err
			}

			// TODO: BOGUS - PINNING TO PINATA AND WRITING MANIFEST FILE
			rng, _ := cache.RangeFromFilename(fileName)
			file.AppendToAsciiFile(config.GetPathToCache(chain)+"tmp/chunks_created.txt", rng.String()+"\n")

			rel := strings.Replace(fileName, config.GetPathToIndex(chain), "$INDEX/", -1)
			result := fmt.Sprintf("%sWrote %d records to %s%s%s", colors.BrightBlue, len(appearanceTable), rel, colors.Off, strings.Repeat(" ", 20))
			if snapper != -1 {
				result = fmt.Sprintf("%sWrote %d records to %s %s(snapped to %d blocks)%s", colors.BrightBlue, len(appearanceTable), rel, colors.Yellow, snapper, colors.Off)
			}
			logger.Log(logger.Info, result)

			// Success. Remove the backup so it doesn't replace the orignal
			os.Remove(backupFn)
			return true, nil
		}
	}

	return false, err
}

type Renderer interface {
	RenderObject(data interface{}, first bool) error
}
