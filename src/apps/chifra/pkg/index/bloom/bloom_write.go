package bloom

import (
	"encoding/binary"
	"io"
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

// TODO: BOGUS - PROTECT AGAINST FAILURE WHEN WRITING
func (bl *ChunkBloom) WriteBloom(chain, fileName string) ( /* changed */ bool, error) {
	var err error

	fileName = ToBloomPath(fileName)
	tmpPath := filepath.Join(config.GetPathToCache(chain), "tmp")

	// Make a backup copy of the file in case the write fails so we can replace it...
	var backupFn string
	if backupFn, err = file.MakeBackup(fileName, tmpPath); err == nil {
		defer func() {
			if file.FileExists(backupFn) {
				// If the backup file exists, something failed, so we replace the original file.
				os.Rename(backupFn, fileName)
				os.Remove(backupFn) // seems redundant, but may not be on some operating systems
			}
		}()

		var fp *os.File
		if fp, err = os.OpenFile(fileName, os.O_RDWR|os.O_CREATE, 0644); err == nil {
			defer fp.Close() // defers are last in, first out

			fp.Seek(0, io.SeekStart) // already true, but can't hurt
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
	}

	return false, err
}
