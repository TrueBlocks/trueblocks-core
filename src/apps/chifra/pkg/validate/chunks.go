package validate

import (
	"encoding/binary"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
)

type ChunkSizes struct {
	BloomSize int64
	IndexSize int64
}

type ErrorType int

const (
	OKAY ErrorType = iota
	FILE_MISSING
	WRONG_SIZE
	WRONG_MAGIC
	WRONG_HASH
	FILE_ERROR
	NOT_IN_MANIFEST
)

// IsValidChunk validates the bloom file's header and the index if told to do so. Note that in all cases, it resolves both.
func IsValidChunk(path string, ch ChunkSizes, indexRequired bool) (ErrorType, ErrorType, error) {
	if path != index.ToBloomPath(path) {
		logger.Fatal("should not happen ==> only process bloom folder paths in IsValidChunk")
	}

	var err error
	indexPath := index.ToIndexPath(path)

	// Resolve the status of the Bloom file first
	bloom := FILE_MISSING
	if file.FileExists(path) {
		bloom = checkSize(path, ch.BloomSize)
		if bloom == OKAY {
			bloom, err = checkHeader(path)
		}
	}
	// The bloom filter is resolved.

	// Determine the status of the index (if it exists)
	index := OKAY
	if !file.FileExists(indexPath) {
		if indexRequired {
			index = FILE_MISSING
		}
	} else {
		index = checkSize(indexPath, ch.IndexSize)
		if index == OKAY {
			index, err = checkHeader(indexPath)
		}
	}

	return bloom, index, err
}

func checkSize(path string, expected int64) ErrorType {
	if !file.FileExists(path) {
		logger.Fatal("should not happen ==> file existence already checked")
	}

	if file.FileSize(path) != expected {
		return WRONG_SIZE
	}

	return OKAY
}

func checkHeader(path string) (ErrorType, error) {
	if !file.FileExists(path) {
		logger.Fatal("should not happen ==> file existence already checked")
	}

	ff, err := os.OpenFile(path, os.O_RDONLY, 0644)
	if err != nil {
		return FILE_ERROR, err
	}
	defer ff.Close()

	if path == index.ToBloomPath(path) {
		var magic uint16
		err = binary.Read(ff, binary.LittleEndian, &magic)
		if err != nil {
			return FILE_ERROR, err
		}
		if magic != file.SmallMagicNumber {
			return WRONG_MAGIC, nil
		}

		var hash base.Hash
		err = binary.Read(ff, binary.LittleEndian, &hash)
		if err != nil {
			return FILE_ERROR, err
		}
		if hash.Hex() != unchained.HeaderMagicHash {
			return WRONG_HASH, nil
		}

		return OKAY, nil

	} else if path == index.ToIndexPath(path) {
		var magic uint32
		err = binary.Read(ff, binary.LittleEndian, &magic)
		if err != nil {
			return FILE_ERROR, err
		}
		if magic != file.MagicNumber {
			return WRONG_MAGIC, nil
		}

		var hash base.Hash
		err = binary.Read(ff, binary.LittleEndian, &hash)
		if err != nil {
			return FILE_ERROR, err
		}
		if hash.Hex() != unchained.HeaderMagicHash {
			return WRONG_HASH, nil
		}

		return OKAY, nil

	} else {
		logger.Fatal("should not happen ==> unknown type in hasValidHeader")
		return OKAY, nil
	}
}
