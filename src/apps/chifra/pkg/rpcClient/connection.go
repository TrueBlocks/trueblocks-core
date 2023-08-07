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

// Settings allows every command has its own options type, we have to
// accept any here, but will use interfaces to read the needed information
type Settings struct {
	Chain         string
	ReadonlyCache bool
	CacheEnabled  bool
	EnabledMap    map[string]bool
	Opts          any
}

func NewConnection(chain string, cacheEnabled bool, enabledMap map[string]bool) *Connection {
	settings := Settings{
		Chain: chain,
		// CacheEnabled: cacheEnabled,
		// EnabledMap:   enabledMap,
	}
	return settings.GetRpcConnection()
}

func TempConnection(chain string) *Connection {
	settings := Settings{
		Chain: chain,
	}
	return settings.GetRpcConnection()
}

func NewReadOnlyConnection(chain string) *Connection {
	settings := Settings{
		Chain:         chain,
		ReadonlyCache: true,
	}
	return settings.GetRpcConnection()
}

// GetRpcConnection builds the store and enables the caches and returns the RPC connection
func (settings Settings) GetRpcConnection() *Connection {
	if cs, ok := settings.Opts.(CacheStater); ok {
		settings.CacheEnabled, settings.EnabledMap = cs.CacheState()
	}
	forceReadonly := !settings.CacheEnabled || settings.ReadonlyCache

	var store *cache.Store
	var err error
	if store, err = cache.NewStore(&cache.StoreOptions{
		Location: cache.FsCache,
		Chain:    settings.Chain,
		ReadOnly: forceReadonly,
	}); err != nil {
		// If there was an error, we won't use the cache
		logger.Warn("Cannot initialize cache:", err)
	}

	ret := &Connection{
		Chain:      settings.Chain,
		Store:      store,
		enabledMap: settings.EnabledMap,
	}

	if store != nil && !store.ReadOnly() {
		ret.LatestBlockTimestamp = ret.GetBlockTimestamp(nil)
	}

	return ret
}

// CacheStater informs us if we should write txs and traces to the cache
type CacheStater interface {
	CacheState() (bool, map[string]bool)
}

// HasStore is a shorthand to check if Store is initialized. It will return
// false for nil pointer to Connection
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

// TestLog prints the enabledMap to the log. Note this routine gets called prior to full initialization, thus it takes the enabledMap
func (conn *Connection) TestLog(enabledMap map[string]bool) {
	if conn.HasStoreWritable() {
		for k, v := range enabledMap {
			logger.TestLog(v, "Cache-"+strings.ToTitle(k)+": ", v)
		}
	}
	// logger.TestLog(options.LatestBlockTimestamp != 0, "LatestBlockTimestamp: ", options.LatestBlockTimestamp)
}

// TODO: This is not consistent with they way we determine unripe in the scraper, for example.
func (conn *Connection) IsFinal(blockTimestamp base.Timestamp) bool {
	var pendingPeriod = int64(5 * 60)
	return (conn.LatestBlockTimestamp - blockTimestamp) >= pendingPeriod
}
