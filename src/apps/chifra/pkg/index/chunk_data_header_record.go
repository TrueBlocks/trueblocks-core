package index

import (
	"encoding/binary"
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
	"github.com/ethereum/go-ethereum/common"
)

// HeaderRecord is the first 44 bytes of an ChunkData. This structure carries a magic number (4 bytes),
// a version specifier (32 bytes), and two four-byte integers representing the number of records in each
// of the two tables.
type HeaderRecord struct {
	Magic           uint32
	Hash            common.Hash
	AddressCount    uint32
	AppearanceCount uint32
}

func ReadHeader(fl *os.File) (header HeaderRecord, err error) {
	err = binary.Read(fl, binary.LittleEndian, &header)
	if err != nil {
		return
	}

	if header.Magic != file.MagicNumber {
		return header, fmt.Errorf("magic number in file %s is incorrect, expected %d, got %d", fl.Name(), file.MagicNumber, header.Magic)
	}

	headerHash := strings.ToLower(header.Hash.Hex())
	if headerHash != unchained.HeaderMagicHash && headerHash != unchained.ZeroMagicHash {
		return header, fmt.Errorf("header hash does not agree in file %s, expected %s, got %s", fl.Name(), unchained.HeaderMagicHash, headerHash)
	}

	return
}

func ReadHeaderFromFilename(fileName string) (header HeaderRecord, err error) {
	fileName = ToIndexPath(fileName)
	ff, err := os.Open(fileName)
	defer ff.Close()
	if err != nil {
		return HeaderRecord{}, err
	}
	return ReadHeader(ff)
}
