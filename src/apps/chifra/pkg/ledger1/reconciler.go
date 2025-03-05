package ledger1

import (
	"encoding/json"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger4"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// Reconciler1 represents the ledger state and provides methods to process and reconcile
// transactions and their associated logs. It holds configuration details such as the
// account being tracked, block ranges for processing, connection to an RPC endpoint,
// asset filters, and maps for both application-level and asset-level contexts.
type Reconciler1 struct {
	accountFor  base.Address
	firstBlock  base.Blknum
	lastBlock   base.Blknum
	names       map[base.Address]types.Name
	testMode    bool
	asEther     bool
	reversed    bool
	useTraces   bool
	connection  *rpc.Connection
	assetFilter []base.Address
}

func (r *Reconciler1) String() string {
	bytes, _ := json.MarshalIndent(r, "", "  ")
	return string(bytes)
}

// NewReconciler1 returns a new empty Reconciler1 struct
func NewReconciler1(opts *ledger4.ReconcilerOptions) *Reconciler1 {
	l := &Reconciler1{
		connection:  opts.Connection,
		accountFor:  opts.AccountFor,
		firstBlock:  opts.FirstBlock,
		lastBlock:   opts.LastBlock,
		asEther:     opts.AsEther,
		testMode:    opts.TestMode,
		reversed:    opts.Reversed,
		useTraces:   opts.UseTraces,
		assetFilter: opts.AssetFilters,
	}

	parts := types.Custom | types.Prefund | types.Regular
	l.names, _ = names.LoadNamesMap(l.connection.Chain, parts, []string{})

	return l
}
