package rpcClient

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cacheNew"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

var NoOptions *Options = nil

// Options carry additional context to rpcClient calls
type Options struct {
	Store                    *cacheNew.Store // Cache Store to use for read/write. Write can be disabled by setting Store to read-only mode
	LatestBlockTimestamp     base.Timestamp
	TransactionWriteDisabled bool // Disable caching transactions
	TraceWriteDisabled       bool // Disable caching traces
}

func (options *Options) TestLog() {
	logger.TestLog(!options.TraceWriteDisabled, "TraceWriteDisabled: ", options.TraceWriteDisabled)
	logger.TestLog(!options.TransactionWriteDisabled, "TransactionWriteDisabled: ", options.TransactionWriteDisabled)
	logger.TestLog(options.LatestBlockTimestamp != 0, "LatestBlockTimestamp", options.LatestBlockTimestamp)
}

func NewConnection(chain string, caches []string) *Options {
	settings := DefaultRpcOptionsSettings{
		Chain: chain,
	}
	return settings.DefaultRpcOptions()
}

func NewReadOnlyConnection(chain string) *Options {
	settings := DefaultRpcOptionsSettings{
		Chain:         chain,
		ReadonlyCache: true,
	}
	return settings.DefaultRpcOptions()
}

func (options *Options) EnableCaches(on, txs, traces bool) {
}

// CacheStore returns cache for the given chain. If readonly is true, it returns
// a cache that will not write new items. If nil is returned, it means "no caching"
func cacheStore(chain string, forceReadonly bool) *cacheNew.Store {
	// We call NewStore, but Storer implementation (Fs by default) should decide
	// whether it has to return a new instance or reuse the existing one
	store, err := cacheNew.NewStore(&cacheNew.StoreOptions{
		Location: cacheNew.FsCache,
		Chain:    chain,
		ReadOnly: forceReadonly,
	})
	// If there was an error, we won't use the cache
	if err != nil {
		logger.Warn("Cannot initialize cache:", err)
		return nil
	}
	return store
}

type DefaultRpcOptionsSettings struct {
	Chain         string
	ReadonlyCache bool
	// Because every command has its own options type, we have to
	// accept any here, but will use interfaces to read the needed
	// information
	Opts any
}

// CacheStater informs us if we should write txs and traces to the cache
type CacheStater interface {
	CacheState() (bool, map[string]bool)
}

func (settings *DefaultRpcOptionsSettings) DefaultRpcOptions() *Options {
	readonlyCache := false
	if settings != nil {
		readonlyCache = settings.ReadonlyCache
	}

	var chain string
	// Check if cache should write
	var cacheEnabled bool
	enabledMap := make(map[string]bool)
	if settings != nil {
		chain = settings.Chain
		if cs, ok := settings.Opts.(CacheStater); ok {
			cacheEnabled, enabledMap = cs.CacheState()
		}
	}

	return &Options{
		Store:                    cacheStore(chain, !cacheEnabled || readonlyCache),
		TransactionWriteDisabled: !enabledMap["txs"],
		TraceWriteDisabled:       !enabledMap["traces"],
	}
}

// HasStore is a shorthand to check if Store is initialized. It will return
// false for nil pointer to Options
func (options *Options) HasStore() bool {
	if options == nil {
		return false
	}

	return options.Store != nil
}

func (options *Options) HasStoreWritable() bool {
	if !options.HasStore() {
		return false
	}

	return !options.Store.ReadOnly()
}
