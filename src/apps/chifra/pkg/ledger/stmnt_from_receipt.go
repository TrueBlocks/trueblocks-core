package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger2"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// getStatementsFromReceipt returns a statement from a given receipt
func (l *Reconciler) getStatementsFromReceipt(pos *types.AppPosition, filter *filter.AppearanceFilter, trans *types.Transaction) ([]types.Statement, error) {
	statements := make([]types.Statement, 0, 20) // a high estimate of the number of statements we'll need
	for _, log := range trans.Receipt.Logs {
		addrArray := []base.Address{l.accountFor}
		if filter.ApplyLogFilter(&log, addrArray) && ledger2.AssetOfInterest(l.assetFilter, log.Address) {
			if s, err := l.getStatementsFromLog(pos, trans, &log); err != nil {
				return statements, err
			} else {
				if (s.Sender == l.accountFor || s.Recipient == l.accountFor) && s.IsMaterial() {
					statements = append(statements, s)
				}
			}
		}
	}

	return statements, nil
}
