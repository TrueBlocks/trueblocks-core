package cache

type CacheItem uint

const (
	ItemABI CacheItem = iota
	ItemBlock
	ItemTransaction
)
