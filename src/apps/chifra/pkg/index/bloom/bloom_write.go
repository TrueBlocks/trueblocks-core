package bloom

import (
	"encoding/binary"
	"io"
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
	"github.com/ethereum/go-ethereum/crypto"
)

// WriteBloom writes a single bloom filter to file
func (bl *ChunkBloom) WriteBloom(chain, bloomFn string) ( /* changed */ bool, error) {
	tmpPath := filepath.Join(config.GetPathToCache(chain), "tmp")

	// Make a backup copy of the file in case the write fails so we can replace it...
	if backupFn, err := file.MakeBackup(tmpPath, bloomFn); err == nil {
		defer func() {
			if file.FileExists(backupFn) {
				// If the backup file exists, something failed, so we replace the original file.
				_ = os.Rename(backupFn, bloomFn)
				_ = os.Remove(backupFn) // seems redundant, but may not be on some operating systems
			}
		}()

		if fp, err := os.OpenFile(bloomFn, os.O_RDWR|os.O_CREATE, 0644); err == nil {
			defer fp.Close() // defers are last in, first out

			_, _ = fp.Seek(0, io.SeekStart) // already true, but can't hurt
			bl.Header.Magic = file.SmallMagicNumber
			bl.Header.Hash = base.BytesToHash(crypto.Keccak256([]byte(version.ManifestVersion)))
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
