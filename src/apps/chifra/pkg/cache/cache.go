package cache

import (
	"bufio"
	"bytes"
	"io"
	"os"
	"path"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	filePkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// Any data structure that we know how to cache
type cacheable interface {
	types.SimpleBlock |
		types.SimpleTransaction
}

// getCacheAndChainPath returns path to cache for given chain
// TODO(cache): I changed the word 'chainName' to 'chain' to be consistent with existing other code
func getCacheAndChainPath(chain string) string {
	cacheDir := config.GetRootConfig().Settings.CachePath
	return path.Join(cacheDir, chain)
}

// save writes contents of `content` Reader to a file
func save(chain string, filePath string, content io.Reader) (err error) {
	cacheDir := getCacheAndChainPath(chain)
	fullPath := path.Join(cacheDir, filePath)

	var file *os.File
	if filePkg.FileExists(fullPath) {
		// If file doesn't exist, we don't need a lock
		file, err = os.OpenFile(fullPath, os.O_RDWR|os.O_CREATE, 0666)
		if err != nil {
			return
		}
		defer file.Close()
	} else {
		file, err = filePkg.WaitOnLock(fullPath, filePkg.DefaultOpenFlags)
		if err != nil {
			return
		}
		defer file.Close()
		if err = filePkg.Empty(file); err != nil {
			return
		}
	}

	_, err = io.Copy(file, content)
	return
}

// load opens binary cache file for reading
func load(chain string, filePath string) (file io.ReadCloser, err error) {
	cacheDir := getCacheAndChainPath(chain)
	fullPath := path.Join(cacheDir, filePath)
	file, err = os.Open(fullPath)
	return
}

func remove(chain string, filePath string) (err error) {
	cacheDir := getCacheAndChainPath(chain)
	fullPath := path.Join(cacheDir, filePath)
	return os.Remove(fullPath)
}

// setItem serializes value into binary format and saves it to a file
func setItem[Data cacheable](
	chain string,
	filePath string,
	value *Data,
	write func(w *bufio.Writer, d *Data) error,
) (err error) {
	buf := bytes.Buffer{}
	writer := bufio.NewWriter(&buf)
	err = write(writer, value)
	if err != nil {
		return
	}
	reader := bytes.NewReader(buf.Bytes())
	err = save(chain, filePath, reader)
	return
}

// getItem reads data structure from binary format
func getItem[Data cacheable](
	chain string,
	filePath string,
	read func(w *bufio.Reader) (*Data, error),
) (value *Data, err error) {
	file, err := load(chain, filePath)
	if err != nil {
		return
	}
	defer file.Close()

	bufReader := bufio.NewReader(file)
	value, err = read(bufReader)
	if err != nil && !os.IsNotExist(err) {
		// Ignore the error, we will re-try next time
		remove(chain, filePath)
	}
	return
}

// SetBlock stores block in the cache
// TODO: Move to it's own type specific file (see https://github.com/TrueBlocks/trueblocks-core/pull/2584#discussion_r1031564867)
// TODO: This also applies to Set/GetTransaction
func SetBlock(chain string, block *types.SimpleBlock) (err error) {
	filePath := getPathByBlock(ItemBlock, block.BlockNumber)

	return setItem(
		chain,
		filePath,
		block,
		WriteBlock,
	)
}

// GetBlock reads block from the cache
func GetBlock(chain string, blockNumber types.Blknum) (block *types.SimpleBlock, err error) {
	filePath := getPathByBlock(ItemBlock, blockNumber)

	return getItem(
		chain,
		filePath,
		ReadBlock,
	)
}

// SetTransaction stores transaction in the cache
func SetTransaction(chain string, tx *types.SimpleTransaction) (err error) {
	filePath := getPathByBlock(ItemTransaction, tx.BlockNumber)

	return setItem(
		chain,
		filePath,
		tx,
		WriteTransaction,
	)
}

// GetTransaction reads transactiono from the cache
func GetTransaction(chain string, blockNumber types.Blknum, txIndex uint64) (tx *types.SimpleTransaction, err error) {
	filePath := getPathByBlockAndTransactionIndex(ItemTransaction, blockNumber, txIndex)

	return getItem(
		chain,
		filePath,
		ReadTransaction,
	)
}
