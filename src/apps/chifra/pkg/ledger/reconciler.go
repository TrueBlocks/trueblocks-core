package ledger

import (
	"encoding/json"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// AssetHolder Struct combining an asset and holder address to key the Running map for
// balance tracking per asset-holder pair.
type AssetHolder struct {
	Asset  base.Address
	Holder base.Address
}

// NewAssetHolderKey Creates a new AssetHolder key.
func NewAssetHolderKey(asset, holder base.Address) AssetHolder {
	return AssetHolder{Asset: asset, Holder: holder}
}

type ReconcilerOptions struct {
	AccountFor   base.Address            `json:"accountFor"`
	FirstBlock   base.Blknum             `json:"firstBlock"`
	LastBlock    base.Blknum             `json:"lastBlock"`
	AsEther      bool                    `json:"asEther"`
	UseTraces    bool                    `json:"useTraces"`
	Reversed     bool                    `json:"reversed"`
	AssetFilters []base.Address          `json:"assetFilters"`
	AppFilters   *types.AppearanceFilter `json:"appFilters"`
}

type Running struct {
	amt  base.Wei
	stmt *types.Statement
}

func (r *Running) String() string {
	return fmt.Sprintf("running blkid: %d amount: %s", r.Block(), r.Amount().Text(10))
}

func (r *Running) Block() base.Blknum {
	if r.stmt == nil {
		return 0
	}
	return r.stmt.BlockNumber
}

func (r *Running) Amount() *base.Wei {
	return &r.amt
}

type Reconciler struct {
	Connection     *rpc.Connection              `json:"-"`
	Opts           *ReconcilerOptions           `json:"opts"`
	Names          map[base.Address]types.Name  `json:"-"`
	ShowDebugging  bool                         `json:"showDebugging"`
	RemoveAirdrops bool                         `json:"removeAirdrops"`
	Running        map[AssetHolder]Running      `json:"running"`
	AssetMap       map[base.Address]*types.Name `json:"assetMap"`
}

func (r *Reconciler) String() string {
	bytes, _ := json.MarshalIndent(r, "", "  ")
	return string(bytes)
}

func NewReconciler(conn *rpc.Connection, opts *ReconcilerOptions) *Reconciler {
	parts := types.Custom | types.Prefund | types.Regular
	names, _ := names.LoadNamesMap("mainnet", parts, []string{})
	r := &Reconciler{
		Opts:           opts,
		Connection:     conn,
		Names:          names,
		ShowDebugging:  true,
		RemoveAirdrops: true,
		Running:        make(map[AssetHolder]Running),
		AssetMap:       make(map[base.Address]*types.Name),
	}
	return r
}
