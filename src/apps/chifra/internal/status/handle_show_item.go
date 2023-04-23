package statusPkg

import (
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
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

	display := path

	switch cT {
	case cache.Index_Maps:
		fallthrough
	case cache.Index_Bloom:
		fallthrough
	case cache.Index_Final:
		if testMode {
			display = strings.Replace(display, config.GetPathToIndex(chain), "$indexPath/", 1)
		}
		fileRange := base.RangeFromFilename(path)
		firstTs, _ := tslib.FromBnToTs(chain, fileRange.First)
		latestTs, _ := tslib.FromBnToTs(chain, fileRange.Last)
		return map[string]interface{}{
			"bloomSizeBytes": file.FileSize(cache.ToBloomPath(path)),
			"fileDate":       date,
			"filename":       display,
			"firstApp":       fileRange.First,
			"firstTs":        firstTs,
			"indexSizeBytes": file.FileSize(cache.ToIndexPath(path)),
			"itemType":       cT.CacheItemName(),
			"latestApp":      fileRange.Last,
			"latestTs":       latestTs,
		}, nil
	case cache.Cache_Monitors:
		fallthrough
	case cache.Cache_Slurps:
		fallthrough
	case cache.Cache_Abis:
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
			"itemType":    cT.CacheItemName(),
			"sizeInBytes": size,
		}
		if cT == cache.Cache_Monitors {
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
			"itemType":    cT.CacheItemName(),
			"sizeInBytes": size,
		}, nil
	}
}
