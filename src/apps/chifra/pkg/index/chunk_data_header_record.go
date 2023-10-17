package index

import (
	"encoding/binary"
	"errors"
	"io"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

// IndexHeader is the first 44 bytes of an ChunkIndex. This structure carries a magic number (4 bytes),
// a version specifier (32 bytes), and two four-byte integers representing the number of records in each
// of the two tables.
type IndexHeader struct {
	Magic           uint32
	Hash            base.Hash
	AddressCount    uint32
	AppearanceCount uint32
}

var ErrIndexHeaderDiffMagic = errors.New("magic number in file is incorrect")
var ErrIndexHeaderDiffHash = errors.New("magic number in file is incorrect")

func X_ReadIndexHeader(fp *os.File, tag string, unused bool) (IndexHeader, error) {
	var header IndexHeader

	_, _ = fp.Seek(0, io.SeekStart) // already true, but can't hurt
	if err := binary.Read(fp, binary.LittleEndian, &header); err != nil {
		return header, err
	}

	if header.Magic != file.MagicNumber {
		return header, ErrIndexHeaderDiffMagic
	}

	if header.Hash.Hex() != tag {
		return header, ErrIndexHeaderDiffHash
	}

	return header, nil
}
