package bloom

import (
	"encoding/binary"
	"fmt"
	"io"
	"os"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

var writeMutex sync.Mutex

func (bl *ChunkBloom) WriteBloom(chain, bloomPath string) error {
	// writeMutex.Lock()
	// trapCh := sigintTrap.Enable(context.WithCancel(context.Background()))

	fmt.Println("In WriteBloom", bloomPath)
	tempPath := strings.Replace(bloomPath, "unchained/sepolia/blooms/", "cache/sepolia/tmp/", -1)
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
	// *bl = ChunkBloom{}
	// bl.ReadBloom(tempPath)

	os.Remove(bloomPath)
	_, err = file.Copy(tempPath, bloomPath)
	if err != nil {
		return err
	}
	return nil
}

// writeBloom
//     output.Write((uint32_t)blooms.size());
//     for (auto bl : blooms) {
//         output.Write(bl.nInserted);
//         output.Write(bl.bits, sizeof(uint8_t), BLOOM_WIDTH_IN_BYTES);
//     }
//     output.Release();
//     unlockSection();
//     return true;
// }
