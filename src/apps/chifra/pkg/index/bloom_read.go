package index

import (
	"encoding/binary"
	"errors"
	"io"
	"os"
	"unsafe"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

// Read reads the entire contents of the bloom filter
func (bl *Bloom) Read(fileName, expectedTag string, unused bool) (err error) {
	bl.Range, err = base.RangeFromFilenameE(fileName)
	if err != nil {
		return err
	}

	bl.File, err = os.OpenFile(fileName, os.O_RDONLY, 0644)
	if err != nil {
		return err
	}
	defer func() {
		bl.File.Close()
		bl.File = nil
	}()

	_, _ = bl.File.Seek(0, io.SeekStart)                      // already true, but can't hurt
	if err = bl.ReadHeader(expectedTag, unused); err != nil { // Note that it may not find a header, but it leaves the file pointer pointing to the count
		return err
	}

	if err = binary.Read(bl.File, binary.LittleEndian, &bl.Count); err != nil {
		return err
	}

	bl.Blooms = make([]bloomBytes, bl.Count)
	for i := uint32(0); i < bl.Count; i++ {
		if err = binary.Read(bl.File, binary.LittleEndian, &bl.Blooms[i].NInserted); err != nil {
			return err
		}

		bl.Blooms[i].Bytes = make([]byte, BLOOM_WIDTH_IN_BYTES)
		if err = binary.Read(bl.File, binary.LittleEndian, &bl.Blooms[i].Bytes); err != nil {
			return err
		}
	}

	return nil
}

var ErrBloomHeaderDiffMagic = errors.New("invalid magic number in bloom header")
var ErrBloomHeaderDiffHash = errors.New("invalid hash in bloom header")

// ReadHeader reads a bloom file header into Bloom.
func (bl *Bloom) ReadHeader(expectedTag string, unused bool) error {

	// Set HeaderSize to 0.
	bl.HeaderSize = 0

	// Read header from file.
	err := binary.Read(bl.File, binary.LittleEndian, &bl.Header)
	if err != nil {
		bl.Header = bloomHeader{}
		_, _ = bl.File.Seek(0, io.SeekStart)
		return err
	}

	// Check for unversioned bloom filter.
	if bl.Header.Magic != file.SmallMagicNumber {
		bl.Header = bloomHeader{}
		_, _ = bl.File.Seek(0, io.SeekStart)
		return ErrBloomHeaderDiffMagic
	}

	// Set HeaderSize.
	bl.HeaderSize = int64(unsafe.Sizeof(bl.Header))

	// Validate hash against provided tag.
	if bl.Header.Hash.Hex() != expectedTag {
		return ErrBloomHeaderDiffHash
	}

	return nil
}
