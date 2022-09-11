package validate

import (
	"encoding/binary"
	"errors"
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/paths"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
	"github.com/ethereum/go-ethereum/common"
)

type IndexHeaderPrefix struct {
	magic uint32
	hash  common.Hash
}

var ErrInvalidMagicNumber = errors.New("file has invalid magic number")
var ErrInvalidHeaderHash = errors.New("file has invalid header hash")

func HasValidHeader(path string) (bool, error) {
	if !file.FileExists(path) {
		msg := fmt.Sprintf("File (%s) not found in HasValidHeader", path)
		return false, errors.New(msg)
	}

	ff, err := os.OpenFile(path, os.O_RDONLY, 0644)
	if err != nil {
		return false, err
	}
	defer ff.Close()

	if path == paths.ToBloomPath(path) {
		var magic uint16
		err = binary.Read(ff, binary.LittleEndian, &magic)
		if err != nil {
			return false, err
		}
		if magic != file.SmallMagicNumber {
			return false, ErrInvalidMagicNumber
		}
		// Read the hash below

	} else if path == paths.ToIndexPath(path) {
		var magic uint32
		err = binary.Read(ff, binary.LittleEndian, &magic)
		if err != nil {
			return false, err
		}
		if magic != file.MagicNumber {
			return false, ErrInvalidMagicNumber
		}
		// Read the hash below

	} else {
		// we don't know what we're reading
		logger.Fatal("should not happen ==> unknown file type in HasValidHeader")
		return false, nil
	}

	var hash common.Hash
	err = binary.Read(ff, binary.LittleEndian, &hash)
	if err != nil {
		return false, err
	}
	if hash.Hex() != unchained.HeaderMagicHash {
		return false, ErrInvalidHeaderHash
	}

	return true, nil
}
