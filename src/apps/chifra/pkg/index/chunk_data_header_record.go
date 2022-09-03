package index

import (
	"encoding/binary"
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"unsafe"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

// HeaderRecord is the first 44 bytes of an ChunkData. This structure carries a magic number (4 bytes),
// a version specifier (32 bytes), and two four-byte integers representing the number of records in each
// of the two tables.
type HeaderRecord struct {
	Magic           uint32
	Hash            common.Hash
	AddressCount    uint32
	AppearanceCount uint32
}

func (h *HeaderRecord) String() string {
	// TODO: BOGUS - USE INDENT IGNORE ERRORS
	b, _ := json.Marshal(h)
	return string(b)
}

func readHeader(fl *os.File) (header HeaderRecord, err error) {
	err = binary.Read(fl, binary.LittleEndian, &header)
	if err != nil {
		return
	}

	// Because we call this frequently, we only check that the magic number is correct
	// we let the caller check the hash if needed
	// TODO: BOGUS - MIGRATION
	if header.Magic != file.MagicNumber {
		return header, fmt.Errorf("magic number in file %s is incorrect, expected %d, got %d", fl.Name(), file.MagicNumber, header.Magic)
	}

	return
}

func ReadChunkHeader(chain, fileName string, checkHash bool) (header HeaderRecord, err error) {
	fileName = config.ToIndexPath(fileName)
	ff, err := os.OpenFile(fileName, os.O_RDONLY, 0)
	if err != nil {
		return HeaderRecord{}, err
	}
	defer ff.Close()

	if header, err = readHeader(ff); err != nil {
		return
	}

	if !checkHash {
		return
	}

	// TODO: BOGUS - MIGRATION
	headerHash := hexutil.Encode(header.Hash.Bytes())
	hasMagicHash := headerHash == unchained.HeaderMagicHash
	if !hasMagicHash {
		return header, fmt.Errorf("header has incorrect hash in %s, expected %s, got %s", fileName, unchained.HeaderMagicHash, headerHash)
	}

	return
}

func WriteChunkHeaderHash(chain, fileName string, headerHash common.Hash) ( /* changed */ bool, error) {
	var err error

	tmpPath := filepath.Join(config.GetPathToCache(chain), "tmp")
	indexFn := config.ToIndexPath(fileName)
	if !file.FileExists(indexFn) {
		return false, nil
	}

	// Make a backup copy of the file in case the write fails so we can replace it...
	if backupFn, err := file.MakeBackup(tmpPath, indexFn); err == nil {
		defer func() {
			if file.FileExists(backupFn) {
				// If the backup file exists, something failed, so we replace the original file.
				os.Rename(backupFn, indexFn)
				os.Remove(backupFn) // seems redundant, but may not be on some operating systems
			}
		}()

		if fp, err := os.OpenFile(indexFn, os.O_RDWR|os.O_CREATE, 0644); err == nil {
			defer fp.Close() // defers are last in, first out

			fp.Seek(0, io.SeekStart) // already true, but can't hurt
			header, err := readHeader(fp)
			if err != nil {
				return false, err
			}

			if header.Hash == headerHash {
				return false, nil
			}

			// We want to write the slice
			// TODO: Not in love with this
			fp.Seek(int64(unsafe.Sizeof(header.Magic)), io.SeekStart)
			err = binary.Write(fp, binary.LittleEndian, headerHash)
			if err != nil {
				return false, err
			}
			fp.Sync() // probably redundant

			// Success. Remove the backup so it doesn't replace the orignal
			os.Remove(backupFn)
			return true, nil
		}
	}

	return false, err
}

// TODO: BOGUS - MIGRATION
func HasValidHeader(chain, fileName string) (bool, error) {
	header, err := ReadChunkHeader(chain, fileName, true)
	if err != nil {
		return false, err
	}

	rng, _ := cache.RangeFromFilename(fileName)
	if header.Magic != file.MagicNumber {
		msg := fmt.Sprintf("%s: Magic number expected (0x%x) got (0x%x)", rng, header.Magic, file.MagicNumber)
		return false, errors.New(msg)

	} else if header.Hash.Hex() != unchained.HeaderMagicHash {
		msg := fmt.Sprintf("%s: Header hash expected (%s) got (%s)", rng, header.Hash.Hex(), unchained.HeaderMagicHash)
		return false, errors.New(msg)
	}

	return true, nil
}
