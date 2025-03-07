package ledger10

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type ReconcilerOptions struct {
	Connection   *rpc.Connection
	AccountFor   base.Address
	FirstBlock   base.Blknum
	LastBlock    base.Blknum
	AsEther      bool
	TestMode     bool
	UseTraces    bool
	Reversed     bool
	AssetFilters []base.Address
}

type AssetTransfer = types.Statement

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
