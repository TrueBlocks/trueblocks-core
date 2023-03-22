package cache

type CacheType uint

const (
	None CacheType = iota
	Index_Bloom
	Index_Final
	Index_Staging
	Index_Ripe
	Index_Unripe
	Cache_Abis
	Cache_Blocks
	Cache_Transactions
	Cache_Traces
)

var cacheDirectories = map[CacheType]string{
	None:               "unknown",
	Index_Bloom:        "blooms",
	Index_Final:        "finalized",
	Index_Staging:      "staging",
	Index_Ripe:         "ripe",
	Index_Unripe:       "unripe",
	Cache_Abis:         "abis",
	Cache_Blocks:       "blocks",
	Cache_Transactions: "txs",
	Cache_Traces:       "traces",
}

func (ct CacheType) String() string {
	descrs := map[CacheType]string{
		None:               "unknown",
		Index_Bloom:        "bloom",
		Index_Final:        "index",
		Index_Staging:      "staging",
		Index_Ripe:         "ripe",
		Index_Unripe:       "unripe",
		Cache_Abis:         "abis",
		Cache_Blocks:       "blocks",
		Cache_Transactions: "transactions",
		Cache_Traces:       "traces",
	}
	return descrs[ct]
}
