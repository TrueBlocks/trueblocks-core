package index

import (
	"encoding/binary"
	"io"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/file"
)

// writeBloom writes a single Bloom filter to file. We do not make a backup copy of the file
// because the caller is responsible for that. This is because the caller may be writing the
// entire chunk (both Bloom and Index) and we want either both to succeed or both to fail.
func (bl *Bloom) writeBloom(fileName string) ( /* changed */ bool, error) {
	var err error
	if bl.File, err = os.OpenFile(fileName, os.O_RDWR|os.O_CREATE, 0644); err == nil {
		defer func() {
			bl.File.Close()
			bl.File = nil
		}()

		_, _ = bl.File.Seek(0, io.SeekStart) // already true, but can't hurt
		bl.Header.Magic = file.SmallMagicNumber
		bl.Header.Hash = base.BytesToHash(config.HeaderHash(config.ExpectedVersion()))

		if err = binary.Write(bl.File, binary.LittleEndian, bl.Header); err != nil {
			return false, err
		}

		if err = binary.Write(bl.File, binary.LittleEndian, bl.Count); err != nil {
			return false, err
		}

		for _, bb := range bl.Blooms {
			if err = binary.Write(bl.File, binary.LittleEndian, bb.NInserted); err != nil {
				return false, err
			}
			if err = binary.Write(bl.File, binary.LittleEndian, bb.Bytes); err != nil {
				return false, err
			}
		}

		return true, nil
	}

	return false, nil
}

// updateTag writes a the header back to the bloom file
func (bl *Bloom) updateTag(tag, fileName string) error {
	var err error
	if bl.File, err = os.OpenFile(fileName, os.O_RDWR, 0644); err != nil {
		return err

	} else {
		defer func() {
			_ = bl.File.Sync()
			_ = bl.File.Close()
			bl.File = nil
		}()

		bl.Header.Magic = file.SmallMagicNumber
		bl.Header.Hash = base.BytesToHash(config.HeaderHash(tag))

		_, _ = bl.File.Seek(0, io.SeekStart) // already true, but can't hurt
		return binary.Write(bl.File, binary.LittleEndian, bl.Header)
	}
}
