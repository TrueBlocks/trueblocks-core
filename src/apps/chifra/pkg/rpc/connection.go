package rpc

import (
	"sort"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// Connection carries additional context to rpc calls
type Connection struct {
	Chain                string
	Store                *cache.Store // Cache Store to use for read/write. Write can be disabled by setting Store to read-only mode
	LatestBlockTimestamp base.Timestamp
	EnabledMap           map[walk.CacheType]bool
	balanceCache         map[string]*base.Wei
	tokenBalanceCache    map[string]*base.Wei
	cacheMutex           sync.Mutex
}

func NewConnection(chain string, cacheEnabled bool, enabledMap map[walk.CacheType]bool) *Connection {
	return newConnection(chain, cacheEnabled, enabledMap)
}

func TempConnection(chain string) *Connection {
	return newConnection(chain, false, nil)
}

// GetRpcConnection builds the store and enables the caches and returns the RPC connection
func newConnection(chain string, cacheEnabled bool, enabledMap map[walk.CacheType]bool) *Connection {
	var store *cache.Store
	var err error
	if store, err = cache.NewStore(&cache.StoreOptions{
		Location: cache.FsCache,
		Chain:    chain,
		Enabled:  cacheEnabled,
	}); err != nil {
		// If there was an error, we won't use the cache
		logger.Warn("Cannot initialize cache:", err)
	}

	ret := &Connection{
		Chain:             chain,
		Store:             store,
		EnabledMap:        enabledMap,
		balanceCache:      make(map[string]*base.Wei),
		tokenBalanceCache: make(map[string]*base.Wei),
	}

	if store != nil && store.Enabled() {
		ret.LatestBlockTimestamp = ret.GetBlockTimestamp(base.NOPOSN)
	}

	return ret
}

// TestLog prints the enabledMap to the log. Note this routine gets called prior to full initialization, thus it takes the enabledMap
func (conn *Connection) TestLog(caches map[walk.CacheType]bool) {
	if conn.Store.Enabled() {
		enabled := []string{}
		for k, v := range caches {
			if v {
				enabled = append(enabled, k.String())
			}
		}
		sort.Strings(enabled)
		logger.TestLog(len(enabled) > 0, "Enabled: ", strings.Join(enabled, ", "))
	}
	// logger.TestLog(options.LatestBlockTimestamp != 0, "LatestBlockTimestamp: ", options.LatestBlockTimestamp)
}
