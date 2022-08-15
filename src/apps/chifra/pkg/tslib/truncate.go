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
