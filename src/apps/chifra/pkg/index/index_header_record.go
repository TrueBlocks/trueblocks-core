package index

import (
	"encoding/binary"
	"fmt"
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
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

func (chunk *Index) readHeader() (indexHeader, error) {
	var header indexHeader

	_, _ = chunk.File.Seek(0, io.SeekStart) // already true, but can't hurt
	if err := binary.Read(chunk.File, binary.LittleEndian, &header); err != nil {
		return header, err
	}

	if header.Magic != file.MagicNumber {
		return header, fmt.Errorf("Index.readHeader: %w %x %x", ErrIncorrectMagic, header.Magic, file.MagicNumber)
	}

	if header.Hash != base.BytesToHash(config.SpecVersionKeccak()) {
		return header, fmt.Errorf("Index.readHeader: %w %x %x", ErrIncorrectHash, header.Hash, base.BytesToHash(config.SpecVersionKeccak()))
	}

	return header, nil
}
