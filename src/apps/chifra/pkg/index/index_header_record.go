package index

import (
	"encoding/binary"
	"fmt"
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/logger"
)

// indexHeader is the first 44 bytes of an Index. This structure carries a magic number (4 bytes),
// a version specifier (32 bytes), and two four-byte integers representing the number of records in each
// of the two tables.
type indexHeader struct {
	Magic           uint32
	Hash            base.Hash
	AddressCount    uint32
	AppearanceCount uint32
}

func (chunk *Index) readHeader(check bool) (indexHeader, error) {
	var header indexHeader

	_, _ = chunk.File.Seek(0, io.SeekStart) // already true, but can't hurt
	if err := binary.Read(chunk.File, binary.LittleEndian, &header); err != nil {
		return header, err
	}

	// always check the magic number
	if header.Magic != file.MagicNumber {
		return header, fmt.Errorf("Index.readHeader: %w %x %x", ErrIncorrectMagic, header.Magic, file.MagicNumber)
	}

	if check { // check if told to do so
		headerHash := header.Hash
		headerTag := config.VersionTags[headerHash.String()]
		expectedHash := base.BytesToHash(config.HeaderHash(config.ExpectedVersion()))
		expectedTag := config.VersionTags[expectedHash.String()]
		if headerHash != expectedHash {
			logger.Warn(fmt.Sprintf(
				"Index header mismatch %s headerHash=0x%x (%s) expectedHash=0x%x (%s)",
				ErrIncorrectHash,
				headerHash, headerTag,
				expectedHash, expectedTag,
			))
		}
	}

	return header, nil
}
