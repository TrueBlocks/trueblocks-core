package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// getStatementsFromReceipt returns a statement from a given receipt
func (l *Ledger) getStatementsFromReceipt(filter *filter.AppearanceFilter, receipt *types.Receipt) ([]types.Statement, error) {
	if receipt == nil {
		return []types.Statement{}, nil
	}

	statements := make([]types.Statement, 0, 20) // a high estimate of the number of statements we'll need
	for _, log := range receipt.Logs {
		addrArray := []base.Address{l.accountFor}
		if filter.ApplyLogFilter(&log, addrArray) && assetOfInterest(l.assetFilter, log.Address) {
			if statement, err := l.getStatementsFromLog(&log); err != nil {
				return statements, err
			} else {
				if statement.Sender == l.accountFor || statement.Recipient == l.accountFor {
					add := !l.noZero || statement.IsMaterial()
					if add {
						statements = append(statements, statement)
					}
				}
			}
		}
	}

	return statements, nil
}
