package tslib

import (
	"encoding/binary"
	"io"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

// var writeMutex sync.Mutex

// TODO: BOGUS - PROTECT AGAINST FAILURE WHEN WRITING
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

	// writeMutex.Lock()
	// trapCh := sigintTrap.Enable(context.WithCancel(context.Background()))

	tempPath := config.GetPathToCache(chain) + "tmp/truncated.bin"
	fp, err := os.OpenFile(tempPath, os.O_WRONLY|os.O_CREATE, 0644)
	defer func() {
		// Clear the cache in even in case of failure, causes a reload at worst
		DeCache(chain)
		os.Remove(tempPath)
		// sigintTrap.Disable(trapCh)
		// writeMutex.Unlock()
	}()
	if err != nil {
		return err
	}

	fp.Seek(0, io.SeekStart)
	err = binary.Write(fp, binary.LittleEndian, truncated)
	if err != nil {
		fp.Close()
		return err
	}

	// Don't defer this because we want it to be closed before we copy it
	fp.Close()

	tsPath := config.GetPathToIndex(chain) + "ts.bin"
	os.Remove(tsPath)
	_, err = file.Copy(tsPath, tempPath)
	if err != nil {
		return err
	}
	return nil
}

/*
	bloomFn := ToBloomPath(fileName)
	tmpPath := filepath.Join(config.GetPathToCache(chain), "tmp")
	if backupFn, err := file.MakeBackup(tmpPath, bloomFn); err == nil {
		defer func() {
			if file.FileExists(backupFn) {
				os.Rename(backupFn, bloomFn)
				os.Remove(backupFn) // seems redundant, but may not be on some operating systems
			}
		}()
		if fp, err := os.OpenFile(bloomFn, os.O_RDWR|os.O_CREATE, 0644); err == nil {
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

			os.Remove(backupFn)
			return true, nil
		}
	} else {
		return false, err
	}
	return false, nil
*/
