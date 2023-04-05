package statusPkg

import (
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
)

func (opts *StatusOptions) getIndexModel(cT cache.CacheType, path string) (map[string]any, error) {
	testMode := opts.Globals.TestMode
	chain := opts.Globals.Chain
	if cT != cache.Index_Bloom && cT != cache.Index_Final && cT != cache.Index_Maps {
		logger.Fatal("Invalid cache type", cT.String())
	}

	d := "--fileDate--"
	info, err := os.Stat(path)
	if !testMode && err == nil {
		d = info.ModTime().Format("2006-01-02 15:04:05")
	}

	bloom := cache.ToBloomPath(path)
	index := cache.ToIndexPath(path)
	fileRange := base.RangeFromFilename(path)

	firstTs, _ := tslib.FromBnToTs(chain, fileRange.First)
	latestTs, _ := tslib.FromBnToTs(chain, fileRange.Last)

	p := path
	if testMode {
		p = strings.Replace(path, config.GetPathToIndex(chain), "$indexPath/", 1)
	}

	model := map[string]interface{}{
		"bloomSizeBytes": file.FileSize(bloom),
		"fileDate":       d,
		"filename":       p,
		"firstApp":       fileRange.First,
		"firstTs":        firstTs,
		"indexSizeBytes": file.FileSize(index),
		"latestApp":      fileRange.Last,
		"latestTs":       latestTs,
		"type":           "indexCacheItem",
	}

	return model, nil
}
