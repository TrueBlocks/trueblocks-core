package ledger

import (
	"encoding/json"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
)

// -----------------------------------------------------------------
// ReconcilerOptions defines the configuration options for the Reconciler.
// It includes filters, block ranges, and appearance settings for reconciliation.
type ReconcilerOptions struct {
	AccountFor   base.Address
	FirstBlock   base.Blknum
	LastBlock    base.Blknum
	AsEther      bool
	UseTraces    bool
	Reversed     bool
	AssetFilters []base.Address
	AppFilters   *types.AppearanceFilter
}

// -----------------------------------------------------------------
// Reconciler performs reconciliation of blockchain data for a specific account.
// It manages connections, options, and intermediate data structures.
type Reconciler struct {
	Connection     *rpc.Connection
	Opts           *ReconcilerOptions
	ShowDebugging  bool
	RemoveAirdrops bool
	Running        map[assetHolder]runningBalance
	AssetMap       map[assetHolder]*types.Name
	Names          map[base.Address]types.Name
}

// -----------------------------------------------------------------
func (r *Reconciler) String() string {
	bytes, _ := json.MarshalIndent(r, "", "  ")
	return string(bytes)
}

// -----------------------------------------------------------------
func NewReconciler(conn *rpc.Connection, opts *ReconcilerOptions) *Reconciler {
	parts := types.Custom | types.Prefund | types.Regular
	names, _ := names.LoadNamesMap("mainnet", parts, []string{})
	r := &Reconciler{
		Opts:           opts,
		Connection:     conn,
		Names:          names,
		ShowDebugging:  true,
		RemoveAirdrops: true,
		Running:        make(map[assetHolder]runningBalance),
		AssetMap:       make(map[assetHolder]*types.Name),
	}
	return r
}
