package rpcClient

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cacheNew"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

var NoOptions *Options = nil

// Options carry additional context to rpcClient calls
type Options struct {
	// Cache Store to use for read/write. Write can be disabled
	// by setting Store to read-only mode
	Store *cacheNew.Store

	LatestBlockTimestamp base.Timestamp
	// Disable caching transactions
	TransactionWriteDisabled bool
	// Disable caching traces
	TraceWriteDisabled bool
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
	CacheState() (bool, bool, bool)
}

func DefaultRpcOptions(settings *DefaultRpcOptionsSettings) *Options {
	readonlyCache := false
	if settings != nil {
		readonlyCache = settings.ReadonlyCache
	}

	var chain string
	// Check if cache should write
	var cacheWriteEnabled bool
	var cacheTxWriteEnabled bool
	var cacheTraceWriteEnabled bool
	if settings != nil {
		chain = settings.Chain
		if cs, ok := settings.Opts.(CacheStater); ok {
			cacheWriteEnabled, cacheTxWriteEnabled, cacheTraceWriteEnabled = cs.CacheState()
		}
	}

	return &Options{
		Store:                    cacheStore(chain, !cacheWriteEnabled || readonlyCache),
		TransactionWriteDisabled: !cacheTxWriteEnabled,
		TraceWriteDisabled:       !cacheTraceWriteEnabled,
	}
}

// HasStore is a shorthand to check if Store is initialized. It will return
// false for nil pointer to Options
func (o *Options) HasStore() bool {
	if o == nil {
		return false
	}

	return o.Store != nil
}

func (o *Options) HasStoreWritable() bool {
	if !o.HasStore() {
		return false
	}

	return !o.Store.ReadOnly()
}
