package rpc

import (
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// Connection carries additional context to rpc calls
type Connection struct {
	Chain string
	Store *cache.Store
	// TODO: BOGUS - THIS IN MEMORY CACHE IS GOOD, BUT COULD BE BINARY FILE
	balanceCache      map[string]*base.Wei
	tokenBalanceCache map[string]*base.Wei
	cacheMutex        sync.Mutex
}

func NewConnection(chain string, cacheEnabled bool, enabledMap map[walk.CacheType]bool) *Connection {
	return newConnection(chain, cacheEnabled, enabledMap, cache.FsCache)
}

func TempConnection(chain string) *Connection {
	return newConnection(chain, false, nil, cache.FsCache)
}

func TestConnection(chain string, onOff bool, caches map[walk.CacheType]bool) *Connection {
	return newConnection(chain, onOff, caches, cache.MemoryCache)
}

// newConnection builds the store and enables the caches and returns the RPC connection
func newConnection(chain string, cacheEnabled bool, enabledMap map[walk.CacheType]bool, loc cache.StoreLocation) *Connection {
	var store *cache.Store
	var err error
	if store, err = cache.NewStore(&cache.StoreOptions{
		Location:   loc,
		Chain:      chain,
		Enabled:    cacheEnabled,
		EnabledMap: enabledMap,
	}); err != nil {
		logger.Warn("Cannot initialize cache:", err)
	}

	ret := &Connection{
		Chain:             chain,
		Store:             store,
		balanceCache:      make(map[string]*base.Wei),
		tokenBalanceCache: make(map[string]*base.Wei),
	}

	if store != nil && store.Enabled() {
		ret.Store.SetLatest(ret.GetBlockTimestamp(base.NOPOSN))
	}

	return ret
}

// TestLog prints the enabledMap to the log. Note this routine gets called prior to full initialization, thus it takes the enabledMap
func (conn *Connection) TestLog() {
	//--to verbose for good testing conn.Store.TestLog()
}

// ReadFromCache is syntactic sugar for the store. Cleans calling code by testing for nil store
func (conn *Connection) ReadFromCache(value cache.Locator) error {
	if conn.Store == nil {
		return nil
	}
	return conn.Store.ReadFromStore(value)
}

// WriteToCache is syntactic sugar for the store. Cleans calling code by testing for nil store
func (conn *Connection) WriteToCache(data cache.Locator, cacheType walk.CacheType, ts base.Timestamp, conditions ...bool) error {
	if conn.Store == nil {
		return nil
	}
	return conn.Store.WriteToStore(data, cacheType, ts, conditions...)
}
