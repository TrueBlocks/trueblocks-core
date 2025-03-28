package index

import (
	"encoding/binary"
	"fmt"
	"io"
	"os"
	"unsafe"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ranges"
)

// Read reads the entire contents of the bloom filter
func (bl *Bloom) Read(fileName string) (err error) {
	bl.Range, err = ranges.RangeFromFilenameE(fileName)
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

	_, _ = bl.File.Seek(0, io.SeekStart)                   // already true, but can't hurt
	if err = bl.readHeader(true /* check */); err != nil { // Note that it may not find a header, but it leaves the file pointer pointing to the count
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

// readHeader reads a bloom file header into Bloom.
func (bl *Bloom) readHeader(check bool) error {

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
		return fmt.Errorf("Bloom.readHeader: %w %x %x", ErrIncorrectMagic, bl.Header.Magic, file.SmallMagicNumber)
	}

	// Set HeaderSize.
	bl.HeaderSize = int64(unsafe.Sizeof(bl.Header))

	// Validate hash against provided tag.
	if check {
		if bl.Header.Hash != base.BytesToHash(config.HeaderHash(config.ExpectedVersion())) {
			return fmt.Errorf("Bloom.readHeader: %w %x %x", ErrIncorrectHash, bl.Header.Hash, base.BytesToHash(config.HeaderHash(config.ExpectedVersion())))
		}
	}

	return nil
}
