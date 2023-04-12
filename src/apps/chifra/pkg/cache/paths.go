package cache

import (
	"fmt"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// TODO(cache): There is code in `chifra monitors --decache` that builds these identical paths. We should
// TODO(cache): comine the at some point in the future. Of course, that code belongs here in the `cache` package.
func getDirStructureByBlock(bn base.Blknum, txId uint64) (dir string, paddedBn string, paddedTx string) {
	paddedBn = fmt.Sprintf("%09d", bn)
	paddedTx = fmt.Sprintf("%05d", txId)

	parts := make([]string, 3)
	parts[0] = paddedBn[:2]
	parts[1] = paddedBn[2:4]
	parts[2] = paddedBn[4:6]
	dir = filepath.Join(parts...)

	return
}

func getPathByBlock(item CacheType, bn base.Blknum) string {
	parentDirs, paddedBn, _ := getDirStructureByBlock(bn, 0)
	directory := cacheTypeToFolder[item]
	extension := cacheTypeToExt[item]
	return filepath.Join(directory, parentDirs, paddedBn+"."+extension)
}

func getPathByBlockAndTransactionIndex(item CacheType, bn base.Blknum, txId uint64) string {
	parentDirs, paddedBn, paddedTx := getDirStructureByBlock(bn, txId)
	directory := cacheTypeToFolder[item]
	extension := cacheTypeToExt[item]
	fileName := strings.Join([]string{paddedBn, paddedTx}, "-")
	return filepath.Join(directory, parentDirs, fileName+"."+extension)
}

func GetRootPathFromCacheType(chain string, cacheType CacheType) string {
	switch cacheType {
	case Cache_Abis:
		fallthrough
	case Cache_Blocks:
		fallthrough
	case Cache_Monitors:
		fallthrough
	case Cache_Names:
		fallthrough
	case Cache_Recons:
		fallthrough
	case Cache_Slurps:
		fallthrough
	case Cache_Tmp:
		fallthrough
	case Cache_Traces:
		fallthrough
	case Cache_Transactions:
		return filepath.Join(config.GetPathToCache(chain), cacheTypeToFolder[cacheType]) + "/"
	case Index_Bloom:
		fallthrough
	case Index_Final:
		fallthrough
	case Index_Ripe:
		fallthrough
	case Index_Staging:
		fallthrough
	case Index_Unripe:
		fallthrough
	case Index_Maps:
		return filepath.Join(config.GetPathToIndex(chain), cacheTypeToFolder[cacheType]) + "/"
	case Cache_NotACache:
		fallthrough
	default:
		logger.Fatal("Should never happen in GetRootPathFromCacheType")
	}

	logger.Fatal("Should never happen in GetRootPathFromCacheType")
	return ""
}
