package cache

type CacheType uint

const (
	Cache_Abis CacheType = iota
	Cache_Blocks
	Cache_Transactions
	Cache_Traces
)

var itemToDirectory = map[CacheType]string{
	Cache_Abis:         "abis",
	Cache_Blocks:       "blocks",
	Cache_Transactions: "txs",
	Cache_Traces:       "traces",
}
