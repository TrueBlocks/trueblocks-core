package ledger2

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// Filter is used to select a subset of ledger data, such as a particular
// asset address, or a specific block range.
type Filter struct {
	AssetAddress string
	MinBlock     base.Blknum
	MaxBlock     base.Blknum
}

// NewFilter returns a Filter with the given asset address, minimum block, and maximum block.
func NewFilter(assetAddress string, minBlock, maxBlock base.Blknum) Filter {
	return Filter{
		AssetAddress: assetAddress,
		MinBlock:     minBlock,
		MaxBlock:     maxBlock,
	}
}

// String returns a human-readable description of this Filter.
func (f *Filter) String() string {
	return "Filter(AssetAddress=" + f.AssetAddress +
		" MinBlock=" + fmt.Sprintf("%d", f.MinBlock) +
		" MaxBlock=" + fmt.Sprintf("%d", f.MaxBlock) + ")"
}

// IsBlockInRange returns true if the given block is between MinBlock and MaxBlock inclusive.
func (f *Filter) IsBlockInRange(block base.Blknum) bool {
	return block >= f.MinBlock && (f.MaxBlock == 0 || block <= f.MaxBlock)
}

// MatchesAsset returns true if the given asset matches this filter's AssetAddress.
// If the filter's AssetAddress is empty, any asset matches.
func (f *Filter) MatchesAsset(assetAddress string) bool {
	if f.AssetAddress == "" {
		return true
	}
	return assetAddress == f.AssetAddress
}
