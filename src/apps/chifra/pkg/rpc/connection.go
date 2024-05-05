package rpc

import (
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// Connection carries additional context to rpc calls
type Connection struct {
	Chain                string
	Store                *cache.Store // Cache Store to use for read/write. Write can be disabled by setting Store to read-only mode
	LatestBlockTimestamp base.Timestamp
	EnabledMap           map[string]bool
}

// settings allows every command has its own options type, we have to
// accept any here, but will use interfaces to read the needed information
type settings struct {
	Chain         string
	ReadonlyCache bool
	CacheEnabled  bool
	EnabledMap    map[string]bool
}

func NewConnection(chain string, cacheEnabled bool, enabledMap map[string]bool) *Connection {
	settings := settings{
		Chain:        chain,
		CacheEnabled: cacheEnabled,
		EnabledMap:   enabledMap,
	}
	return settings.GetRpcConnection()
}

func TempConnection(chain string) *Connection {
	settings := settings{
		Chain: chain,
	}
	return settings.GetRpcConnection()
}

func NewReadOnlyConnection(chain string) *Connection {
	settings := settings{
		Chain:         chain,
		ReadonlyCache: true,
	}
	return settings.GetRpcConnection()
}

// GetRpcConnection builds the store and enables the caches and returns the RPC connection
func (settings settings) GetRpcConnection() *Connection {
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
		EnabledMap: settings.EnabledMap,
	}

	if store != nil && !store.ReadOnly() {
		ret.LatestBlockTimestamp = ret.GetBlockTimestamp(base.NOPOSN2)
	}

	return ret
}

// StoreReadable is a shorthand to check if Store is initialized. It will return
// false for nil pointer to Connection
func (conn *Connection) StoreReadable() bool {
	if conn == nil {
		logger.Fatal("should not happen ==> implementation error in StoreReadable.")
	}

	return conn.Store != nil
}

func (conn *Connection) StoreWritable() bool {
	if !conn.StoreReadable() {
		return false
	}
	return !conn.Store.ReadOnly()
}

// TestLog prints the enabledMap to the log. Note this routine gets called prior to full initialization, thus it takes the enabledMap
func (conn *Connection) TestLog(enabledMap map[string]bool) {
	if conn.StoreWritable() {
		enabled := []string{}
		for k, v := range enabledMap {
			if v {
				enabled = append(enabled, k)
			}
		}
		sort.Strings(enabled)
		logger.TestLog(len(enabled) > 0, "Enabled: ", strings.Join(enabled, ", "))
	}
	// logger.TestLog(options.LatestBlockTimestamp != 0, "LatestBlockTimestamp: ", options.LatestBlockTimestamp)
}
