package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// TODO: Two things to note. (1) if balances were part of this structure, we could fill those
// TODO: balances in a concurrent way before spinning through the appearances. And (2) if we did that
// TODO: prior to doing the accounting, we could easily travers in reverse order.

// Reconciler represents the ledger state and provides methods to process and reconcile
// transactions and their associated logs. It holds configuration details such as the
// account being tracked, block ranges for processing, connection to an RPC endpoint,
// asset filters, and maps for both application-level and asset-level contexts.
type Reconciler struct {
	accountFor  base.Address
	firstBlock  base.Blknum
	lastBlock   base.Blknum
	names       map[base.Address]types.Name
	testMode    bool
	asEther     bool
	noZero      bool
	reversed    bool
	useTraces   bool
	connection  *rpc.Connection
	assetFilter []base.Address
}

// NewLedger returns a new empty Reconciler struct
func NewLedger(conn *rpc.Connection, acctFor base.Address, fb, lb base.Blknum, asEther, testMode, noZero, useTraces, reversed bool, assetFilters *[]string) *Reconciler {
	l := &Reconciler{
		connection: conn,
		accountFor: acctFor,
		firstBlock: fb,
		lastBlock:  lb,
		asEther:    asEther,
		testMode:   testMode,
		noZero:     noZero,
		reversed:   reversed,
		useTraces:  useTraces,
	}

	if assetFilters != nil {
		l.assetFilter = make([]base.Address, len(*assetFilters))
		for i, addr := range *assetFilters {
			l.assetFilter[i] = base.HexToAddress(addr)
		}
	} else {
		l.assetFilter = []base.Address{}
	}

	parts := types.Custom | types.Prefund | types.Regular
	l.names, _ = names.LoadNamesMap(conn.Chain, parts, []string{})

	return l
}
