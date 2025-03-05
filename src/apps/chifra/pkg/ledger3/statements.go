package ledger3

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// GetStatements returns a statement from a given transaction
func (r *Reconciler3) GetStatements(pos *types.AppPosition, filter *filter.AppearanceFilter, trans *types.Transaction) ([]types.Statement, error) {
	return []types.Statement{}, nil // r.GetAssetTransfers(pos, filter, trans)
}
