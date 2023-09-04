package tslib

import (
	"encoding/binary"
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

func Append(chain string, tsArray []TimestampRecord) error {
	tsFn := config.GetPathToIndex(chain) + "ts.bin"
	tmpPath := filepath.Join(config.GetPathToCache(chain), "tmp")
	if backupFn, err := file.MakeBackup(tmpPath, tsFn); err == nil {
		ClearCache(chain)
		defer func() {
			if file.FileExists(backupFn) {
				_ = os.Rename(backupFn, tsFn)
				_ = os.Remove(backupFn) // seems redundant, but may not be on some operating systems
			}
		}()

		fp, err := os.OpenFile(tsFn, os.O_WRONLY|os.O_CREATE|os.O_APPEND, 0644)
		if err != nil {
			return err
		}
		defer func() {
			fp.Close()
		}()

		err = binary.Write(fp, binary.LittleEndian, tsArray)
		if err != nil {
			return err
		}

		os.Remove(backupFn)
		return nil

	} else {
		return err
	}
}
