package tslib

import (
	"encoding/binary"
	"errors"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"unsafe"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
)

// Repair repairs a single timestamp
func Repair(chain string, bn base.Blknum) error {
	cnt, err := NTimestamps(chain)
	if err != nil {
		return err
	}
	if bn >= cnt {
		msg := fmt.Sprintf("Block number %d out of range %d", bn, cnt)
		return errors.New(msg)
	}

	tsFn := config.PathToTimestamps(chain)
	tmpPath := filepath.Join(config.PathToCache(chain), "tmp")
	if backupFn, err := file.MakeBackup(tmpPath, tsFn); err == nil {
		defer func() {
			ClearCache(chain)
			if file.FileExists(backupFn) {
				// If the backup file exists, something failed, so we replace the original file.
				_ = os.Rename(backupFn, tsFn)
				_ = os.Remove(backupFn) // seems redundant, but may not be on some operating systems
			}
		}()

		if fp, err := os.OpenFile(tsFn, os.O_RDWR|os.O_CREATE, 0644); err == nil {
			defer func() { // defers are last in, first out
				fp.Close()
				// sigintTrap.Disable(trapCh)
				// writeMutex.Unlock()
			}()

			recordSize := int64(unsafe.Sizeof(uint32(0))) * 2
			pos := (recordSize * int64(bn))
			_, _ = fp.Seek(pos, io.SeekStart)

			conn := rpc.TempConnection(chain)
			block, _ := conn.GetBlockHeaderByNumber2(bn)
			record := TimestampRecord{Bn: uint32(block.BlockNumber), Ts: uint32(block.Timestamp)}
			err = binary.Write(fp, binary.LittleEndian, &record)
			if err != nil {
				return err
			}
			_ = fp.Sync() // probably redundant

			os.Remove(backupFn)
			return nil
		} else {
			return err
		}
	} else {
		return err
	}
}
