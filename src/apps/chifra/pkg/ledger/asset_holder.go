package ledger

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"

// -----------------------------------------------------------------
// assetHolder represents a unique combination of an asset and its holder.
type assetHolder struct {
	Asset  base.Address
	Holder base.Address
}

// -----------------------------------------------------------------
func newAssetHolderKey(asset, holder base.Address) assetHolder {
	return assetHolder{Asset: asset, Holder: holder}
}
