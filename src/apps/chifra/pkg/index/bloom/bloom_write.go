package bloom

import (
	"encoding/binary"
	"io"
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

// WriteBloom writes a single bloom filter to file
func (bl *ChunkBloom) WriteBloom(chain, tag, fileName string, unused bool) ( /* changed */ bool, error) {
	tmpPath := filepath.Join(config.PathToCache(chain), "tmp")

	// Make a backup copy of the file in case the write fails so we can replace it...
	if backupFn, err := file.MakeBackup(tmpPath, fileName); err == nil {
		defer func() {
			if file.FileExists(backupFn) {
				// If the backup file exists, something failed, so we replace the original file.
				_ = os.Rename(backupFn, fileName)
				_ = os.Remove(backupFn) // seems redundant, but may not be on some operating systems
			}
		}()

		if fp, err := os.OpenFile(fileName, os.O_RDWR|os.O_CREATE, 0644); err == nil {
			defer fp.Close() // defers are last in, first out

			_, _ = fp.Seek(0, io.SeekStart) // already true, but can't hurt
			bl.Header.Magic = file.SmallMagicNumber
			bl.Header.Hash = base.BytesToHash([]byte(tag))
			if err = binary.Write(fp, binary.LittleEndian, bl.Header); err != nil {
				return false, err
			}

			if err = binary.Write(fp, binary.LittleEndian, bl.Count); err != nil {
				return false, err
			}

			for _, bb := range bl.Blooms {
				if err = binary.Write(fp, binary.LittleEndian, bb.NInserted); err != nil {
					return false, err
				}
				if err = binary.Write(fp, binary.LittleEndian, bb.Bytes); err != nil {
					return false, err
				}
			}

			// Success. Remove the backup so it doesn't replace the orignal
			os.Remove(backupFn)
			return true, nil
		}
	} else {
		return false, err
	}

	return false, nil
}

// UpdateBloomHeader writes a the header back to the bloom file
func UpdateBloomHeader(chain, tag, fileName string, unused bool) error {
	if fp, err := os.OpenFile(fileName, os.O_RDWR|os.O_CREATE, 0644); err != nil {
		return err
	} else {
		defer fp.Close() // defers are last in, first out

		var header BloomHeader
		header.Magic = file.SmallMagicNumber
		header.Hash = base.BytesToHash([]byte(tag))

		_, _ = fp.Seek(0, io.SeekStart) // already true, but can't hurt
		return binary.Write(fp, binary.LittleEndian, header)
	}
}
