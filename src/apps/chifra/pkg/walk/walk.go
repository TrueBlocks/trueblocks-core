// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package walk

import (
	"context"
	"io/fs"
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ranges"
)

type CacheType uint

const (
	Cache_NotACache CacheType = iota
	Cache_Abis
	Cache_Monitors
	Cache_Names
	Cache_Tmp

	Cache_Blocks
	Cache_Logs
	Cache_Receipts
	Cache_Results
	Cache_Slurps
	Cache_State
	Cache_Statements
	Cache_Tokens
	Cache_Traces
	Cache_Transactions
	Cache_Withdrawals

	Index_Bloom
	Index_Final
	Index_Ripe
	Index_Staging
	Index_Unripe
	Index_Maps

	Config
	Regular
)

var cacheTypeToName = map[CacheType]string{
	Cache_NotACache:    "unknown",
	Cache_Abis:         "abis",
	Cache_Monitors:     "monitors",
	Cache_Names:        "names",
	Cache_Tmp:          "tmp",
	Cache_Blocks:       "blocks",
	Cache_Logs:         "logs",
	Cache_Receipts:     "receipts",
	Cache_Results:      "results",
	Cache_Slurps:       "slurps",
	Cache_State:        "state",
	Cache_Statements:   "statements",
	Cache_Tokens:       "tokens",
	Cache_Traces:       "traces",
	Cache_Transactions: "transactions",
	Cache_Withdrawals:  "withdrawals",
	Index_Bloom:        "bloom",
	Index_Final:        "index",
	Index_Ripe:         "ripe",
	Index_Staging:      "staging",
	Index_Unripe:       "unripe",
	Index_Maps:         "neighbors",
	Config:             "config",
	Regular:            "regular",
}

// CacheTypeToFolder is a map of cache types to the folder name (also, it acts as the mode in chifra status)
var CacheTypeToFolder = map[CacheType]string{
	Cache_NotACache:    "unknown",
	Cache_Abis:         "abis",
	Cache_Monitors:     "monitors",
	Cache_Names:        "names",
	Cache_Tmp:          "tmp",
	Cache_Blocks:       "blocks",
	Cache_Logs:         "logs",
	Cache_Receipts:     "receipts",
	Cache_Results:      "results",
	Cache_Slurps:       "slurps",
	Cache_State:        "state",
	Cache_Statements:   "statements",
	Cache_Tokens:       "tokens",
	Cache_Traces:       "traces",
	Cache_Transactions: "transactions",
	Cache_Withdrawals:  "withdrawals",
	Index_Bloom:        "blooms",
	Index_Final:        "finalized",
	Index_Ripe:         "ripe",
	Index_Staging:      "staging",
	Index_Unripe:       "unripe",
	Index_Maps:         "maps",
	Config:             "config",
	Regular:            "regular",
}

var CacheTypeToExt = map[CacheType]string{
	Cache_NotACache:    "unknown",
	Cache_Abis:         "json",
	Cache_Monitors:     "mon.bin",
	Cache_Names:        "bin",
	Cache_Tmp:          "",
	Cache_Blocks:       "bin",
	Cache_Logs:         "bin",
	Cache_Receipts:     "bin",
	Cache_Results:      "bin",
	Cache_Slurps:       "bin",
	Cache_State:        "bin",
	Cache_Statements:   "bin",
	Cache_Tokens:       "bin",
	Cache_Traces:       "bin",
	Cache_Transactions: "bin",
	Cache_Withdrawals:  "bin",
	Index_Bloom:        "bloom",
	Index_Final:        "bin",
	Index_Ripe:         "txt",
	Index_Staging:      "txt",
	Index_Unripe:       "txt",
	Index_Maps:         "bin",
	Config:             "toml",
	Regular:            "",
}

func (ct CacheType) String() string {
	return cacheTypeToName[ct]
}

func IsCacheType(path string, cT CacheType, checkExt bool) bool {
	if !strings.Contains(path, CacheTypeToFolder[cT]) {
		return false
	}
	if checkExt && !strings.HasSuffix(path, CacheTypeToExt[cT]) {
		return false
	}
	return true
}

func GetRootPathFromCacheType(chain string, cacheType CacheType) string {
	switch cacheType {
	case Cache_Monitors:
		fallthrough
	case Cache_Names:
		fallthrough
	case Cache_Tmp:
		return filepath.Join(config.PathToCache(chain), CacheTypeToFolder[cacheType])

	case Cache_Abis:
		return filepath.Join(config.PathToCache(chain), CacheTypeToFolder[cacheType])
	case Cache_Blocks:
		fallthrough
	case Cache_Logs:
		fallthrough
	case Cache_Receipts:
		fallthrough
	case Cache_Results:
		fallthrough
	case Cache_Slurps:
		fallthrough
	case Cache_State:
		fallthrough
	case Cache_Statements:
		fallthrough
	case Cache_Tokens:
		fallthrough
	case Cache_Traces:
		fallthrough
	case Cache_Transactions:
		fallthrough
	case Cache_Withdrawals:
		return filepath.Join(config.PathToCache(chain), "v1", CacheTypeToFolder[cacheType])

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
		return filepath.Join(config.PathToIndex(chain), CacheTypeToFolder[cacheType])
	case Config:
		return config.PathToRootConfig()
	case Cache_NotACache:
		fallthrough
	default:
		logger.Fatal("should not happen ==> in paths.go")
	}

	logger.Fatal("should not happen ==> in paths.go")
	return ""
}

func WalkCacheFolder(ctx context.Context, chain string, cacheType CacheType, data interface{}, filenameChan chan<- CacheFileInfo) {
	path := GetRootPathFromCacheType(chain, cacheType)
	walkFolder(ctx, path, cacheType, data, filenameChan)
}

func WalkConfigFolders(ctx context.Context, data interface{}, filenameChan chan<- CacheFileInfo) {
	path := config.PathToRootConfig()
	walkFolder(ctx, path, Config, data, filenameChan)
}

func WalkFolder(ctx context.Context, path string, data interface{}, filenameChan chan<- CacheFileInfo) {
	walkFolder(ctx, path, Regular, data, filenameChan)
}

func walkFolder(ctx context.Context, path string, cacheType CacheType, data interface{}, filenameChan chan<- CacheFileInfo) {
	defer func() {
		filenameChan <- CacheFileInfo{Type: Cache_NotACache}
	}()

	_ = filepath.Walk(path, func(path string, info fs.FileInfo, err error) error {
		if err != nil {
			// If the scraper is running, this will sometimes send an error for a file, for example, that existed
			// when it was first seen, but the scraper deletes before this call. We ignore any file system errors
			// this routine, but if we experience problems, we can uncomment this line
			// fmt.Printf("prevent panic by handling failure accessing a path %q: %v\n", path, err)
			return err
		}

		if info.IsDir() {
			filenameChan <- CacheFileInfo{Type: cacheType, Path: path, IsDir: true, Data: data}

		} else {
			// TODO: This does not need to be part of walker. It could be in the caller and sent through the data pointer
			rng := ranges.RangeFromFilename(path)
			filenameChan <- CacheFileInfo{Type: cacheType, Path: path, FileRange: rng, Data: data}
		}

		select {
		case <-ctx.Done():
			return ctx.Err()
		default:
		}

		return nil
	})
}

func CacheTypesFromStringSlice(strs []string) []CacheType {
	haveit := map[string]bool{} // removes dups
	var types []CacheType
	for _, str := range strs {
		if !haveit[str] {
			haveit[str] = true
			switch str {
			case "abis":
				types = append(types, Cache_Abis) // ABIS_CACHE_PATH
			case "monitors":
				types = append(types, Cache_Monitors)
			case "names":
				types = append(types, Cache_Names)
			case "tmp":
				types = append(types, Cache_Tmp)

			case "blocks":
				types = append(types, Cache_Blocks)
			case "logs":
				types = append(types, Cache_Logs)
			case "receipts":
				types = append(types, Cache_Receipts)
			case "results":
				types = append(types, Cache_Results)
			case "slurps":
				types = append(types, Cache_Slurps)
			case "state":
				types = append(types, Cache_State)
			case "statements":
				types = append(types, Cache_Statements)
			case "tokens":
				types = append(types, Cache_Tokens)
			case "traces":
				types = append(types, Cache_Traces)
			case "transactions":
				types = append(types, Cache_Transactions)
			case "withdrawals":
				types = append(types, Cache_Withdrawals)

			case "blooms":
				types = append(types, Index_Bloom)
			case "index":
				fallthrough
			case "finalized":
				types = append(types, Index_Final)
			case "ripe":
				types = append(types, Index_Ripe)
			case "staging":
				types = append(types, Index_Staging)
			case "unripe":
				types = append(types, Index_Unripe)
			case "maps":
				types = append(types, Index_Maps)
			case "some":
				types = append(types, Index_Final)
				types = append(types, Cache_Monitors)
				types = append(types, Cache_Names)
				types = append(types, Cache_Abis)
				types = append(types, Cache_Slurps)
			case "all":
				types = append(types, Index_Bloom)
				types = append(types, Index_Final)
				types = append(types, Index_Staging)
				types = append(types, Index_Unripe)
				types = append(types, Cache_Abis)
				types = append(types, Cache_Monitors)
				types = append(types, Cache_Names)
				types = append(types, Cache_Blocks)
				types = append(types, Cache_Logs)
				types = append(types, Cache_Receipts)
				types = append(types, Cache_Results)
				types = append(types, Cache_Slurps)
				types = append(types, Cache_State)
				types = append(types, Cache_Statements)
				types = append(types, Cache_Tokens)
				types = append(types, Cache_Traces)
				types = append(types, Cache_Transactions)
				types = append(types, Cache_Withdrawals)
			}
		}
	}
	return types
}

func GetCacheItem(chain string, testMode bool, cT CacheType, cacheInfo *CacheFileInfo) (map[string]any, error) {
	date := "--fileDate--"
	info, err := os.Stat(cacheInfo.Path)
	if !testMode && err == nil {
		date = info.ModTime().Format("2006-01-02 15:04:05")
	}

	size := file.FileSize(cacheInfo.Path)
	if testMode {
		size = 123456789
	}

	display := cacheInfo.Path
	display = filepath.Clean(strings.ReplaceAll(display, config.PathToCache(chain)+"/", "./"))
	display = filepath.Clean(strings.ReplaceAll(display, config.PathToIndex(chain)+"/", "./"))

	switch cT {
	case Index_Maps:
		fallthrough
	case Index_Bloom:
		fallthrough
	case Index_Final:
		if testMode {
			display = strings.Replace(cacheInfo.Path, config.PathToIndex(chain)+"/", "$indexPath/", 1)
		}
		return map[string]any{
			// "bloomSizeBytes": file.FileSize(index.ToBloomPath(cacheInfo.Path)),
			"fileDate": date,
			"filename": display,
			"firstApp": cacheInfo.FileRange.First,
			"firstTs":  cacheInfo.TsRange.First,
			// "indexSizeBytes": file.FileSize(index.ToIndexPath(cacheInfo.Path)),
			"itemType":  WalkCacheName(cT) + "Item",
			"latestApp": cacheInfo.FileRange.Last,
			"latestTs":  cacheInfo.TsRange.Last,
		}, nil
	case Cache_Monitors:
		fallthrough
	case Cache_Slurps:
		fallthrough
	case Cache_Abis:
		address := ""
		parts := strings.Split(cacheInfo.Path, string(os.PathSeparator))
		for _, part := range parts {
			if strings.HasPrefix(part, "0x") {
				address = part
				break
			}
		}
		if testMode {
			display = strings.Replace(cacheInfo.Path, config.PathToCache(chain)+"/", "$cachePath/", 1)
			display = strings.Replace(display, address, "--address--", -1)
			address = "--address--"
		}
		ret := map[string]any{
			"address":     address,
			"fileDate":    date,
			"filename":    display,
			"itemType":    WalkCacheName(cT) + "Item",
			"sizeInBytes": size,
		}
		if cT == Cache_Monitors {
			ret["nRecords"] = size / 8 // index.AppRecordWidth - FAST
		}
		return ret, nil
	default:
		if testMode {
			display = "$cachePath/data-model/file.bin"
		}
		return map[string]any{
			"fileDate":    date,
			"filename":    display,
			"itemType":    WalkCacheName(cT) + "Item",
			"sizeInBytes": size,
		}, nil
	}
}

func WalkCacheName(ct CacheType) string {
	// TODO: Names of caches, names of folders, names of commands are all different. This is a mess.
	ret := CacheTypeToFolder[ct] + "Cache"
	ret = strings.Replace(ret, "blooms", "bloom", -1)
	ret = strings.Replace(ret, "finalized", "index", -1)
	return ret
}

type CacheFileInfo struct {
	Type      CacheType
	FileRange ranges.FileRange
	TsRange   ranges.TimestampRange
	Path      string
	IsDir     bool
	Data      interface{}
}
