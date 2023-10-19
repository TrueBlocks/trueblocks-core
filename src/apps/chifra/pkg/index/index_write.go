package index

import (
	"encoding/binary"
	"io"
	"os"
	"unsafe"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/ethereum/go-ethereum/crypto"
)

// updateTag updates both the index and the bloom filter headers for a chunk.
// This is a non-recoverable operation. The caller must take care of making a backup of
// the file before we start if desired.
func (idx *Index) updateTag(newTag, fileName string) error {
	var err error
	if idx.File, err = os.OpenFile(fileName, os.O_RDWR, 0644); err != nil {
		return err

	} else {
		defer func() {
			_ = idx.File.Sync() // probably redundant
			_ = idx.File.Close()
			idx.File = nil
		}()

		tagBytes := base.BytesToHash(crypto.Keccak256([]byte(newTag)))

		// don't love this, but it saves us from having to read in and preserve the header
		_, _ = idx.File.Seek(int64(unsafe.Sizeof(idx.Header.Magic)), io.SeekStart)
		if err = binary.Write(idx.File, binary.LittleEndian, tagBytes); err != nil {
			return err
		}
	}

	return nil
}
