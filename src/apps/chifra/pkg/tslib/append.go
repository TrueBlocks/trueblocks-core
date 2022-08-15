package tslib

import (
	"encoding/binary"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

// TODO: BOGUS - PROTECT AGAINST FAILURE WHEN WRITING
func Append(chain string, tsArray []Timestamp) error {
	tsPath := config.GetPathToIndex(chain) + "ts.bin"
	fp, err := os.OpenFile(tsPath, os.O_WRONLY|os.O_APPEND|os.O_CREATE, 0644)
	defer func() {
		DeCache(chain)
		fp.Close()
		// sigintTrap.Disable(trapCh)
		// writeMutex.Unlock()
	}()
	if err != nil {
		return err
	}

	// fp.Seek(0, io.SeekStart)
	err = binary.Write(fp, binary.LittleEndian, tsArray)
	if err != nil {
		return err
	}

	return nil
}
