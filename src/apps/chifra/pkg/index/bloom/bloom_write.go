package bloom

import (
	"encoding/binary"
	"io"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

// var writeMutex sync.Mutex

func (bl *ChunkBloom) WriteBloom(chain, bloomPath string) error {
	// writeMutex.Lock()
	// trapCh := sigintTrap.Enable(context.WithCancel(context.Background()))

	// TODO: BOGUS - YIKES
	tempPath := strings.Replace(bloomPath, "unchained/sepolia/blooms/", "cache/sepolia/tmp/", -1)
	tempPath = strings.Replace(tempPath, "unchained/gnosis/blooms/", "cache/gnosis/tmp/", -1)
	tempPath = strings.Replace(tempPath, "unchained/mainnet/blooms/", "cache/mainnet/tmp/", -1)
	tempPath = strings.Replace(tempPath, "unchained/polygon/blooms/", "cache/polygon/tmp/", -1)
	fp, err := os.OpenFile(tempPath, os.O_WRONLY|os.O_CREATE, 0644)
	defer func() {
		os.Remove(tempPath)
		// sigintTrap.Disable(trapCh)
		// writeMutex.Lock()
	}()
	if err != nil {
		return err
	}

	fp.Seek(0, io.SeekStart)
	err = binary.Write(fp, binary.LittleEndian, bl.Count)
	if err != nil {
		fp.Close()
		return err
	}

	for _, bb := range bl.Blooms {
		err = binary.Write(fp, binary.LittleEndian, bb.NInserted)
		if err != nil {
			fp.Close()
			return err
		}
		err = binary.Write(fp, binary.LittleEndian, bb.Bytes)
		if err != nil {
			fp.Close()
			return err
		}
	}
	// Don't defer this because we want it to be closed before we copy it
	fp.Close()

	os.Remove(bloomPath)
	_, err = file.Copy(tempPath, bloomPath)
	if err != nil {
		return err
	}
	return nil
}
