package statusPkg

import (
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
)

func (opts *StatusOptions) getCacheItem(cT cache.CacheType, path string) (map[string]any, error) {
	testMode := opts.Globals.TestMode
	chain := opts.Globals.Chain

	date := "--fileDate--"
	info, err := os.Stat(path)
	if !testMode && err == nil {
		date = info.ModTime().Format("2006-01-02 15:04:05")
	}

	size := file.FileSize(path)
	if testMode {
		size = 123456789
	}

	indexDisplay := path
	if testMode {
		indexDisplay = strings.Replace(indexDisplay, config.GetPathToIndex(chain), "$indexPath/", 1)
	}

	address := ""
	parts := strings.Split(path, "/")
	for _, part := range parts {
		if strings.HasPrefix(part, "0x") {
			address = part
			break
		}
	}

	cacheDisplay := path
	if testMode {
		cacheDisplay = strings.Replace(cacheDisplay, config.GetPathToCache(chain), "$cachePath/", 1)
		if len(address) > 0 {
			cacheDisplay = strings.Replace(cacheDisplay, address, "--address--", -1)
			address = "--address--"
		} else if strings.Contains(path, "txs") || strings.Contains(path, "traces") || strings.Contains(path, "blocks") {
			cacheDisplay = "$cachePath/data-model/file.bin"
		}
	}

	switch cT {
	case cache.Index_Maps:
		fallthrough
	case cache.Index_Bloom:
		fallthrough
	case cache.Index_Final:
		fileRange := base.RangeFromFilename(path)
		firstTs, _ := tslib.FromBnToTs(chain, fileRange.First)
		latestTs, _ := tslib.FromBnToTs(chain, fileRange.Last)
		return map[string]interface{}{
			"bloomSizeBytes": file.FileSize(cache.ToBloomPath(path)),
			"fileDate":       date,
			"filename":       indexDisplay,
			"firstApp":       fileRange.First,
			"firstTs":        firstTs,
			"indexSizeBytes": file.FileSize(cache.ToIndexPath(path)),
			"latestApp":      fileRange.Last,
			"latestTs":       latestTs,
			"itemType":       cT.CacheTypeToStr() + "CacheItem",
		}, nil
	case cache.Cache_Monitors:
		fallthrough
	case cache.Cache_Slurps:
		fallthrough
	case cache.Cache_Abis:
		return map[string]interface{}{
			"address":     address,
			"fileDate":    date,
			"filename":    cacheDisplay,
			"sizeInBytes": size,
			"itemType":    cT.CacheTypeToStr() + "CacheItem",
		}, nil
	default:
		return map[string]interface{}{
			"fileDate":    date,
			"filename":    cacheDisplay,
			"sizeInBytes": size,
			"itemType":    cT.CacheTypeToStr() + "CacheItem",
		}, nil
	}
}
