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
	Chain             string
	Store             *cache.Store
	balanceCache      map[string]*base.Wei
	tokenBalanceCache map[string]*base.Wei
	cacheMutex        sync.Mutex
}

func NewConnection(chain string, cacheEnabled bool, enabledMap map[walk.CacheType]bool) *Connection {
	return newConnection(chain, cacheEnabled, enabledMap)
}

func TempConnection(chain string) *Connection {
	return newConnection(chain, false, nil)
}

// newConnection builds the store and enables the caches and returns the RPC connection
func newConnection(chain string, cacheEnabled bool, enabledMap map[walk.CacheType]bool) *Connection {
	var store *cache.Store
	var err error
	if store, err = cache.NewStore(&cache.StoreOptions{
		Location:   cache.FsCache,
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
		ret.Store.Latest = ret.GetBlockTimestamp(base.NOPOSN)
	}

	return ret
}

// TestLog prints the enabledMap to the log. Note this routine gets called prior to full initialization, thus it takes the enabledMap
func (conn *Connection) TestLog() {
	if conn.Store.Enabled() {
		enabled := []string{}
		for k, v := range conn.Store.EnabledMap {
			if v {
				enabled = append(enabled, k.String())
			}
		}
		sort.Strings(enabled)
		logger.TestLog(len(enabled) > 0, "Enabled: ", strings.Join(enabled, ", "))
	}
}
