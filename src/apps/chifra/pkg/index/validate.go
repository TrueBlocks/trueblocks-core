package index

import (
	"encoding/binary"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
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

var Reasons = map[ErrorType]string{
	OKAY:            "okay",
	FILE_ERROR:      "file error",
	FILE_MISSING:    "file missing",
	WRONG_SIZE:      "wrong size",
	WRONG_MAGIC:     "wrong magic number",
	WRONG_HASH:      "wrong header hash",
	NOT_IN_MANIFEST: "not in manifest",
}

// IsValidChunk validates the bloom file's header and the index if told to do so. Note that in all cases, it resolves both.
func IsValidChunk(path string, ch ChunkSizes, indexRequired bool) (ErrorType, ErrorType, error) {
	if path != ToBloomPath(path) {
		logger.Fatal("should not happen ==> only process bloom folder paths in IsValidChunk")
	}

	var err error
	indexPath := ToIndexPath(path)

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

	if path == ToBloomPath(path) {
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

	} else if path == ToIndexPath(path) {
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
