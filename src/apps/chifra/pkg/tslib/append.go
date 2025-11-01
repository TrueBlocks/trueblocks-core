package tslib

import (
	"encoding/binary"
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/file"
)

func Append(chain string, tsArray []TimestampRecord) error {
	tsFn := config.PathToTimestamps(chain)
	tmpPath := filepath.Join(config.PathToCache(chain), "tmp")
	if backup, err := file.MakeBackup(tmpPath, tsFn); err == nil {
		ClearCache(chain)
		defer func() {
			backup.Restore()
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

		backup.Clear()
		return nil

	} else {
		return err
	}
}
