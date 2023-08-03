package statusPkg

import (
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func (opts *StatusOptions) getCacheItem(cT walk.CacheType, path string) (map[string]any, error) {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode

	date := "--fileDate--"
	info, err := os.Stat(path)
	if !testMode && err == nil {
		date = info.ModTime().Format("2006-01-02 15:04:05")
	}

	size := file.FileSize(path)
	if testMode {
		size = 123456789
	}

	display := path

	switch cT {
	case walk.Index_Maps:
		fallthrough
	case walk.Index_Bloom:
		fallthrough
	case walk.Index_Final:
		if testMode {
			display = strings.Replace(display, config.GetPathToIndex(chain), "$indexPath/", 1)
		}
		fileRange := base.RangeFromFilename(path)
		firstTs, _ := tslib.FromBnToTs(chain, fileRange.First)
		latestTs, _ := tslib.FromBnToTs(chain, fileRange.Last)
		return map[string]interface{}{
			"bloomSizeBytes": file.FileSize(index.ToBloomPath(path)),
			"fileDate":       date,
			"filename":       display,
			"firstApp":       fileRange.First,
			"firstTs":        firstTs,
			"indexSizeBytes": file.FileSize(index.ToIndexPath(path)),
			"itemType":       cacheItemName(cT),
			"latestApp":      fileRange.Last,
			"latestTs":       latestTs,
		}, nil
	case walk.Cache_Monitors:
		fallthrough
	case walk.Cache_Slurps:
		fallthrough
	case walk.Cache_Abis:
		address := ""
		parts := strings.Split(path, "/")
		for _, part := range parts {
			if strings.HasPrefix(part, "0x") {
				address = part
				break
			}
		}
		if testMode {
			display = strings.Replace(display, config.GetPathToCache(chain), "$cachePath/", 1)
			display = strings.Replace(display, address, "--address--", -1)
			address = "--address--"
		}
		ret := map[string]interface{}{
			"address":     address,
			"fileDate":    date,
			"filename":    display,
			"itemType":    cacheItemName(cT),
			"sizeInBytes": size,
		}
		if cT == walk.Cache_Monitors {
			ret["nRecords"] = size / index.AppRecordWidth // TODO: 8 bytes per record so we don't have to read the file
		}
		return ret, nil
	default:
		if testMode {
			display = "$cachePath/data-model/file.bin"
		}
		return map[string]interface{}{
			"fileDate":    date,
			"filename":    display,
			"itemType":    cacheItemName(cT),
			"sizeInBytes": size,
		}, nil
	}
}

func cacheItemName(ct walk.CacheType) string {
	return cacheName(ct) + "Item"
}

func cacheName(ct walk.CacheType) string {
	// TODO: Names of caches, names of folders, names of commands are all different. This is a mess.
	ret := walk.CacheTypeToFolder[ct] + "Cache"
	ret = strings.Replace(ret, "blooms", "bloom", -1)
	ret = strings.Replace(ret, "finalized", "index", -1)
	return ret
}
