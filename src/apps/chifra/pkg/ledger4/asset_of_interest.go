package ledger4

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"

// AssetOfInterest returns true if the asset filter is empty or the asset matches
func AssetOfInterest(filters []base.Address, needle base.Address) bool {
	if len(filters) == 0 {
		return true
	}

	for _, asset := range filters {
		if asset.Hex() == needle.Hex() {
			return true
		}
	}

	return false
}
