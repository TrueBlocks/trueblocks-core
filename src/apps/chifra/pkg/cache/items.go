package cache

import (
	"strings"
	"sync"
)

type CacheType uint

const (
	Cache_NotACache CacheType = iota
	Cache_Abis
	Cache_Blocks
	Cache_Monitors
	Cache_Names
	Cache_Recons
	Cache_Slurps
	Cache_Tmp
	Cache_Traces
	Cache_Transactions
	Index_Bloom
	Index_Final
	Index_Ripe
	Index_Staging
	Index_Unripe
	Index_Maps
)

var cacheTypeToName = map[CacheType]string{
	Cache_NotACache:    "unknown",
	Cache_Abis:         "abis",
	Cache_Blocks:       "blocks",
	Cache_Monitors:     "monitors",
	Cache_Names:        "names",
	Cache_Recons:       "reconciliations",
	Cache_Slurps:       "slurps",
	Cache_Tmp:          "tmp",
	Cache_Traces:       "traces",
	Cache_Transactions: "transactions",
	Index_Bloom:        "bloom",
	Index_Final:        "index",
	Index_Ripe:         "ripe",
	Index_Staging:      "staging",
	Index_Unripe:       "unripe",
	Index_Maps:         "neighbors",
}

// cacheTypeToFolder is a map of cache types to the folder name (also, it acts as the mode in chifra status)
var cacheTypeToFolder = map[CacheType]string{
	Cache_NotACache:    "unknown",
	Cache_Abis:         "abis",
	Cache_Blocks:       "blocks",
	Cache_Monitors:     "monitors",
	Cache_Names:        "names",
	Cache_Recons:       "recons",
	Cache_Slurps:       "slurps",
	Cache_Tmp:          "tmp",
	Cache_Traces:       "traces",
	Cache_Transactions: "txs",
	Index_Bloom:        "blooms",
	Index_Final:        "finalized",
	Index_Ripe:         "ripe",
	Index_Staging:      "staging",
	Index_Unripe:       "unripe",
	Index_Maps:         "maps",
}

var cacheTypeToExt = map[CacheType]string{
	Cache_NotACache:    "unknown",
	Cache_Abis:         "json",
	Cache_Blocks:       "bin",
	Cache_Monitors:     "mon.bin",
	Cache_Names:        "bin",
	Cache_Recons:       "bin",
	Cache_Slurps:       "bin",
	Cache_Tmp:          "",
	Cache_Traces:       "bin",
	Cache_Transactions: "bin",
	Index_Bloom:        "bloom",
	Index_Final:        "bin",
	Index_Ripe:         "txt",
	Index_Staging:      "txt",
	Index_Unripe:       "txt",
	Index_Maps:         "bin",
}

func (ct CacheType) String() string {
	return cacheTypeToName[ct]
}

var strToCacheType = map[string]CacheType{}
var strToCacheTypeMutex sync.RWMutex

func StrToCacheType(s string) CacheType {
	if len(strToCacheType) == 0 {
		strToCacheTypeMutex.Lock()
		defer strToCacheTypeMutex.Unlock()
		for k, v := range cacheTypeToFolder {
			strToCacheType[v] = k
		}
	}
	return strToCacheType[s]
}

func (ct CacheType) CacheName() string {
	// TODO: Names of caches, names of folders, names of commands are all different. This is a mess.
	ret := cacheTypeToFolder[ct] + "Cache"
	ret = strings.Replace(ret, "blooms", "bloom", -1)
	ret = strings.Replace(ret, "finalized", "index", -1)
	return ret
}

func (ct CacheType) CacheItemName() string {
	return ct.CacheName() + "Item"
}

func IsCacheType(path string, cT CacheType, checkExt bool) bool {
	if !strings.Contains(path, cacheTypeToFolder[cT]) {
		return false
	}
	if checkExt && !strings.HasSuffix(path, cacheTypeToExt[cT]) {
		return false
	}
	return true
}

func GetCacheTypes(strs []string) []CacheType {
	haveit := map[string]bool{} // removes dups
	var types []CacheType
	for _, str := range strs {
		if !haveit[str] {
			haveit[str] = true
			switch str {
			case "abis":
				types = append(types, Cache_Abis)
			case "blocks":
				types = append(types, Cache_Blocks)
			case "monitors":
				types = append(types, Cache_Monitors)
			case "names":
				types = append(types, Cache_Names)
			case "recons":
				types = append(types, Cache_Recons)
			case "slurps":
				types = append(types, Cache_Slurps)
			case "tmp":
				types = append(types, Cache_Tmp)
			case "traces":
				types = append(types, Cache_Traces)
			case "txs":
				types = append(types, Cache_Transactions)
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
				types = append(types, Cache_Monitors)
				types = append(types, Cache_Names)
				types = append(types, Cache_Abis)
				types = append(types, Cache_Slurps)
				types = append(types, Cache_Blocks)
				types = append(types, Cache_Traces)
				types = append(types, Cache_Transactions)
			}
		}
	}
	/*
		all:     abis|monitors|names|slurps|blocks|traces|txs|recons|tmp|blooms|index|finalized|staging|ripe|unripe|maps
		cmd:     abis|monitors|names|slurps|blocks|traces|txs|index|some|all
		missing: recons|tmp|blooms|finalized|staging|ripe|unripe|maps
	*/
	return types
}

func IsIndexType(cT CacheType) bool {
	m := map[CacheType]bool{
		Index_Bloom:   true,
		Index_Final:   true,
		Index_Ripe:    true,
		Index_Staging: true,
		Index_Unripe:  true,
		Index_Maps:    true,
	}
	return m[cT]
}
