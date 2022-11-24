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
