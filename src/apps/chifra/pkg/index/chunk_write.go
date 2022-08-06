package index

import (
	"encoding/binary"
	"fmt"
	"io"
	"log"
	"os"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
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
func WriteChunk(chain, indexPath string, addAppMap AddressAppearanceMap, nApps, snapper int) (uint64, error) {
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

	rel := strings.Replace(indexPath, config.GetPathToIndex(chain), "$INDEX/", -1)

	tempPath := strings.Replace(indexPath, "/unchained/", "/cache/", -1)
	tempPath = strings.Replace(tempPath, "/finalized/", "/tmp/", -1)
	if indexPath == tempPath {
		log.Fatal("Paths should differ:", tempPath, indexPath)
	}
	fp, err := os.OpenFile(tempPath, os.O_WRONLY|os.O_CREATE, 0644)
	defer func() {
		os.Remove(tempPath)
		// sigintTrap.Disable(trapCh)
		// writeMutex.Lock()
	}()
	if err != nil {
		return 0, err
	}

	fp.Seek(0, io.SeekStart)
	header := HeaderRecord{
		Magic:           file.MagicNumber,
		Hash:            common.BytesToHash(crypto.Keccak256([]byte(version.ManifestVersion))),
		AddressCount:    uint32(len(addressTable)),
		AppearanceCount: uint32(len(appearanceTable)),
	}
	err = binary.Write(fp, binary.LittleEndian, header)
	if err != nil {
		fp.Close()
		return 0, err
	}
	err = binary.Write(fp, binary.LittleEndian, addressTable)
	if err != nil {
		fp.Close()
		return 0, err
	}
	err = binary.Write(fp, binary.LittleEndian, appearanceTable)
	if err != nil {
		fp.Close()
		return 0, err
	}
	// Don't defer this because we want it to be closed before we copy it
	fp.Close()

	snapMsg := ""
	if snapper != -1 {
		snapMsg = fmt.Sprintf("(isSnap to modulo %d blocks)", snapper)
	}
	logger.Log(logger.Info, "Wrote", len(appearanceTable), "records to", rel, snapMsg)

	os.Remove(indexPath)
	_, err = file.Copy(indexPath, tempPath)
	if err != nil {
		return 0, err
	}

	err = bl.WriteBloom(chain, bloom.ToBloomPath(indexPath))
	if err != nil {
		return 0, err
	}

	// TODO: BOGUS - PINNING TO PINATA AND WRITING MANIFEST FILE
	rng, _ := cache.RangeFromFilename(indexPath)
	file.AppendToAsciiFile(config.GetPathToCache(chain)+"tmp/chunks_created.txt", rng.String()+"\n")

	return 0, nil
}

type Renderer interface {
	RenderObject(data interface{}, first bool) error
}
