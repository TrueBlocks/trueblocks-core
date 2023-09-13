package tslib

import (
	"encoding/binary"
	"io"
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

// var writeMutex sync.Mutex

func Truncate(chain string, maxBn uint64) error {
	cnt, err := NTimestamps(chain)
	if err != nil {
		return err
	}

	// It's already done
	if maxBn >= cnt {
		return nil
	}

	err = loadTimestamps(chain)
	if err != nil {
		return err
	}

	truncated := perChainTimestamps[chain].memory[0:maxBn]

	tsFn := filepath.Join(config.GetPathToIndex(chain), "ts.bin")
	tmpPath := filepath.Join(config.GetPathToCache(chain), "tmp")
	if backupFn, err := file.MakeBackup(tmpPath, tsFn); err == nil {
		defer func() {
			ClearCache(chain)
			if file.FileExists(backupFn) {
				// If the backup file exists, something failed, so we replace the original file.
				_ = os.Rename(backupFn, tsFn)
				_ = os.Remove(backupFn) // seems redundant, but may not be on some operating systems
			}
		}()
		if fp, err := os.OpenFile(tsFn, os.O_WRONLY|os.O_CREATE, 0644); err == nil {
			defer func() {
				fp.Close()
			}()

			_, _ = fp.Seek(0, io.SeekStart)
			err = binary.Write(fp, binary.LittleEndian, truncated)
			if err != nil {
				return err
			}
			_ = fp.Sync()

			os.Remove(backupFn)
			return nil
		} else {
			return err
		}
	} else {
		return err
	}
}
