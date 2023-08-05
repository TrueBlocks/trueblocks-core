package rpcClient

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// Connection carries additional context to rpcClient calls
type Connection struct {
	Chain                string
	Store                *cache.Store // Cache Store to use for read/write. Write can be disabled by setting Store to read-only mode
	LatestBlockTimestamp base.Timestamp
	enabledMap           map[string]bool
}

// TestLog prints the enabledMap to the log. Note this routine gets called prior to full initialization, thus it takes the enabledMap
func (conn *Connection) TestLog(enabledMap map[string]bool) {
	if conn.HasStoreWritable() {
		for k, v := range enabledMap {
			logger.TestLog(v, "Cache-"+strings.ToTitle(k)+": ", v)
		}
	}
	// logger.TestLog(options.LatestBlockTimestamp != 0, "LatestBlockTimestamp: ", options.LatestBlockTimestamp)
}

func NewConnection(chain string, enabled bool, enabledMap map[string]bool) *Connection {
	settings := ConnectionSettings{
		Chain: chain,
	}
	return settings.DefaultRpcOptions()
}

func TempConnection(chain string) *Connection {
	settings := ConnectionSettings{
		Chain: chain,
	}
	return settings.DefaultRpcOptions()
}

func NewReadOnlyConnection(chain string) *Connection {
	settings := ConnectionSettings{
		Chain:         chain,
		ReadonlyCache: true,
	}
	return settings.DefaultRpcOptions()
}

// CacheStore returns cache for the given chain. If readonly is true, it returns
// a cache that will not write new items. If nil is returned, it means "no caching"
//
// We call NewStore, but Storer implementation (Fs by default) should decide
// whether it has to return a new instance or reuse the existing one
func cacheStore(chain string, forceReadonly bool) *cache.Store {
	if store, err := cache.NewStore(&cache.StoreOptions{
		Location: cache.FsCache,
		Chain:    chain,
		ReadOnly: forceReadonly,
	}); err != nil {
		// If there was an error, we won't use the cache
		logger.Warn("Cannot initialize cache:", err)
		return nil
	} else {
		return store
	}
}

// CacheStater informs us if we should write txs and traces to the cache
type CacheStater interface {
	CacheState() (bool, map[string]bool)
}

// ConnectionSettings allows every command has its own options type, we have to
// accept any here, but will use interfaces to read the needed information
type ConnectionSettings struct {
	Chain         string
	ReadonlyCache bool
	Opts          any
}

func (settings ConnectionSettings) DefaultRpcOptions() *Connection {
	readonlyCache := settings.ReadonlyCache

	var chain string
	var cacheEnabled bool
	enabledMap := make(map[string]bool)
	chain = settings.Chain
	if cs, ok := settings.Opts.(CacheStater); ok {
		cacheEnabled, enabledMap = cs.CacheState()
	}

	store := cacheStore(chain, !cacheEnabled || readonlyCache)
	ret := &Connection{
		Chain:      chain,
		Store:      store,
		enabledMap: enabledMap,
	}
	if !store.ReadOnly() {
		ret.LatestBlockTimestamp = ret.GetBlockTimestamp(nil)
	}

	return ret
}

// HasStore is a shorthand to check if Store is initialized. It will return
// false for nil pointer to Options
func (conn *Connection) HasStore() bool {
	if conn == nil {
		return false
	}
	return conn.Store != nil
}

func (conn *Connection) HasStoreWritable() bool {
	if !conn.HasStore() {
		return false
	}
	return !conn.Store.ReadOnly()
}
