package cache

import (
	"os"
	"path"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

func IsAbiCacheUpToDate(chain string) (upToDate bool, err error) {
	cacheFilePath := path.Join(
		config.GetPathToCache(chain),
		"abis/known.bin",
	)
	cacheFile, err := os.Stat(cacheFilePath)
	if os.IsNotExist(err) {
		return false, nil
	}
	if err != nil {
		return
	}
	knownDir := path.Join(
		config.GetPathToRootConfig(),
		"abis",
	)

	newestFile, err := file.GetNewestInDirectory(knownDir)
	if err != nil {
		return
	}
	return cacheFile.ModTime().Unix() >= newestFile.ModTime().Unix(), nil
}
