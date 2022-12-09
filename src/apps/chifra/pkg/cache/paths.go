package cache

import (
	"fmt"
	"path"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var itemToDirectory = map[CacheItem]string{
	ItemABI:         "abis",
	ItemBlock:       "blocks",
	ItemTransaction: "txs",
}

func getDirectoryByItem(item CacheItem) string {
	return itemToDirectory[item]
}

// TODO(cache): There is code in `chifra monitors --decache` that builds these identical paths. We should
// TODO(cache): comine the at some point in the future. Of course, that code belongs here in the `cache` package.
func getDirStructureByBlock(blockNumber types.Blknum) (result string, paddedBn string) {
	paddedBn = fmt.Sprintf("%09d", blockNumber)
	parts := make([]string, 3)
	parts[0] = paddedBn[:2]
	parts[1] = paddedBn[2:4]
	parts[2] = paddedBn[4:6]
	result = path.Join(parts...)
	return
}

func getPathByBlock(item CacheItem, blockNumber types.Blknum) string {
	parentDirs, bn := getDirStructureByBlock(blockNumber)
	directory := getDirectoryByItem(item)
	return path.Join(directory, parentDirs, bn+".bin")
}

func getPathByBlockAndTransactionIndex(
	item CacheItem,
	blockNumber types.Blknum,
	txId uint64,
) string {
	txIndex := fmt.Sprintf("%05d", txId)
	parentDirs, bn := getDirStructureByBlock(blockNumber)
	directory := getDirectoryByItem(item)
	return path.Join(directory, parentDirs, strings.Join(
		[]string{bn, txIndex},
		"-",
	)+".bin")
}
