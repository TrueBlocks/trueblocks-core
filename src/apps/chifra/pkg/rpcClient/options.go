package rpcClient

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cacheNew"
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
