package cache

import (
	"strings"
)

type CacheType uint

const (
	None CacheType = iota
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

func (ct CacheType) String() string {
	descrs := map[CacheType]string{
		None:               "unknown",
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
	return descrs[ct]
}

var cacheDirectories = map[CacheType]string{
	None:               "unknown",
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

var cacheExtensions = map[CacheType]string{
	None:               "unknown",
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

func IsCacheType(path string, cT CacheType) bool {
	if !strings.Contains(path, cacheDirectories[cT]) {
		return false
	}
	if !strings.HasSuffix(path, cacheExtensions[cT]) {
		return false
	}
	return true
}
