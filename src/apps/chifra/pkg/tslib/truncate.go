package tslib

import (
	"encoding/binary"
	"io"
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/file"
)

// var writeMutex sync.Mutex

func Truncate(chain string, maxBn base.Blknum) error {
	cnt, err := NTimestamps(chain)
	if err != nil {
		return err
	}

	// It's already done
	if maxBn >= base.Blknum(cnt) {
		return nil
	}

	err = loadTimestamps(chain)
	if err != nil {
		return err
	}

	truncated := perChainTimestamps[chain].memory[0:maxBn]

	tsFn := config.PathToTimestamps(chain)
	tmpPath := filepath.Join(config.PathToCache(chain), "tmp")
	if backup, err := file.MakeBackup(tmpPath, tsFn); err == nil {
		defer func() {
			ClearCache(chain)
			backup.Restore()
		}()

		// remove the file and...
		if err := os.Remove(tsFn); err != nil {
			return err
		}

		// ...open it fresh
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

			backup.Clear()
			return nil
		} else {
			return err
		}
	} else {
		return err
	}
}
