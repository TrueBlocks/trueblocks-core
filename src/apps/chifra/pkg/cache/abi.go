package cache

import (
	"path"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

func IsAbiCacheUpToDate(chain string) (upToDate bool, err error) {
	cacheDir := config.GetPathToCache(chain)
	abiCacheDir := path.Join(
		cacheDir,
		"abis",
	)

	newestFile, err := file.GetNewstInDirectory(abiCacheDir)
	if err != nil {
		return
	}
	return newestFile.Name() == "known.bin", nil
}
