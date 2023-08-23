package index

import (
	"encoding/binary"
	"encoding/json"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"unsafe"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
)

// IndexHeaderRecord is the first 44 bytes of an ChunkData. This structure carries a magic number (4 bytes),
// a version specifier (32 bytes), and two four-byte integers representing the number of records in each
// of the two tables.
type IndexHeaderRecord struct {
	Magic           uint32
	Hash            base.Hash
	AddressCount    uint32
	AppearanceCount uint32
}

func (h *IndexHeaderRecord) String() string {
	b, _ := json.MarshalIndent(h, "", " ")
	return string(b)
}

func readIndexHeader(fl *os.File) (header IndexHeaderRecord, err error) {
	err = binary.Read(fl, binary.LittleEndian, &header)
	if err != nil {
		return
	}

	// Because we call this frequently, we only check that the magic number is correct
	// we let the caller check the hash if needed
	if header.Magic != file.MagicNumber {
		return header, fmt.Errorf("magic number in file %s is incorrect, expected %d, got %d", fl.Name(), file.MagicNumber, header.Magic)
	}

	return
}

func ReadChunkHeader(fileName string, checkHash bool) (header IndexHeaderRecord, err error) {
	fileName = ToIndexPath(fileName)
	ff, err := os.OpenFile(fileName, os.O_RDONLY, 0)
	if err != nil {
		return IndexHeaderRecord{}, err
	}
	defer ff.Close()

	if header, err = readIndexHeader(ff); err != nil {
		return
	}

	if !checkHash {
		return
	}

	headerHash := header.Hash.Hex()
	hasMagicHash := headerHash == unchained.HeaderMagicHash
	if !hasMagicHash {
		return header, fmt.Errorf("header has incorrect hash in %s, expected %s, got %s", fileName, unchained.HeaderMagicHash, headerHash)
	}

	return
}

func WriteChunkHeaderHash(chain, fileName string, headerHash base.Hash) ( /* changed */ bool, error) {
	var err error

	tmpPath := filepath.Join(config.GetPathToCache(chain), "tmp")
	indexFn := ToIndexPath(fileName)
	if !file.FileExists(indexFn) {
		return false, nil
	}

	// Make a backup copy of the file in case the write fails so we can replace it...
	if backupFn, err := file.MakeBackup(tmpPath, indexFn); err == nil {
		defer func() {
			if file.FileExists(backupFn) {
				// If the backup file exists, something failed, so we replace the original file.
				_ = os.Rename(backupFn, indexFn)
				_ = os.Remove(backupFn) // seems redundant, but may not be on some operating systems
			}
		}()

		if fp, err := os.OpenFile(indexFn, os.O_RDWR|os.O_CREATE, 0644); err == nil {
			defer fp.Close() // defers are last in, first out

			_, _ = fp.Seek(0, io.SeekStart) // already true, but can't hurt
			header, err := readIndexHeader(fp)
			if err != nil {
				return false, err
			}

			if header.Hash == headerHash {
				return false, nil
			}

			// We want to write the slice
			// TODO: I do not like this code
			_, _ = fp.Seek(int64(unsafe.Sizeof(header.Magic)), io.SeekStart)
			err = binary.Write(fp, binary.LittleEndian, headerHash)
			if err != nil {
				return false, err
			}
			_ = fp.Sync() // probably redundant

			// Success. Remove the backup so it doesn't replace the orignal
			os.Remove(backupFn)
			return true, nil
		}
	}

	return false, err
}
