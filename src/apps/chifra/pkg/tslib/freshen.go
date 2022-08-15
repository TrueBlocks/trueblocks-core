package tslib

import (
	"encoding/binary"
	"errors"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"unsafe"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
)

// TODO: BOGUS - PROTECT AGAINST FAILURE WHEN WRITING
func Freshen(chain string, bn uint64) error {
	cnt, err := NTimestamps(chain)
	if err != nil {
		return err
	}
	if bn >= cnt {
		msg := fmt.Sprintf("Block number %d out of range %d", bn, cnt)
		return errors.New(msg)
	}

	tmpPath := filepath.Join(config.GetPathToCache(chain), "tmp")
	fileName := filepath.Join(config.GetPathToIndex(chain), "ts.bin")

	// Make a backup copy of the file in case the write fails so we can replace it...
	if backupFn, err := file.MakeBackup(fileName, tmpPath); err == nil {
		defer func() {
			DeCache(chain)
			if file.FileExists(backupFn) {
				// If the backup file exists, something failed, so we replace the original file.
				os.Rename(backupFn, fileName)
				os.Remove(backupFn) // seems redundant, but may not be on some operating systems
			}
		}()

		if fp, err := os.OpenFile(fileName, os.O_RDWR|os.O_CREATE, 0644); err == nil {
			if err != nil {
				return err
			}
			defer func() { // defers are last in, first out
				fp.Close()
				// sigintTrap.Disable(trapCh)
				// writeMutex.Unlock()
			}()

			// TODO: Not in love with this
			recordSize := int64(unsafe.Sizeof(uint32(0))) * 2
			pos := (recordSize * int64(bn))
			fp.Seek(pos, io.SeekStart)

			block, _ := rpcClient.GetBlockByNumber(chain, bn)
			record := Timestamp{Bn: uint32(block.BlockNumber), Ts: uint32(block.TimeStamp)}
			err = binary.Write(fp, binary.LittleEndian, &record)
			if err != nil {
				return err
			}
			fp.Sync() // probably redundant

			// Success. Remove the backup so it doesn't replace the orignal
			os.Remove(backupFn)
			return nil
		}
	}
	return err
}
