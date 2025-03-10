package ledger1

import (
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// GetTransfers1 returns a statement from a given transaction
func (r *Reconciler1) GetTransfers1(pos *types.AppPosition, trans *types.Transaction) ([]types.Transfer, error) {
	r.enabledCorrections = false
	if statements, err := r.GetStatements1(pos, trans); err != nil {
		return nil, err
	} else {
		return convertToTransfers(statements)
	}
}

func convertToTransfers(statements []types.Statement) ([]types.Transfer, error) {
	transfers := make([]types.Transfer, 0, len(statements)*2)
	for _, stmnt := range statements {
		t := types.Transfer{
			Asset:            stmnt.Asset,
			Holder:           stmnt.AccountedFor,
			Amount:           *stmnt.AmountNet(),
			BlockNumber:      stmnt.BlockNumber,
			TransactionIndex: stmnt.TransactionIndex,
			LogIndex:         stmnt.LogIndex,
			Decimals:         stmnt.Decimals,
			CorrectingReason: stmnt.CorrectingReason,
		}
		transfers = append(transfers, t)
	}

	if base.IsTestMode() {
		sort.Slice(transfers, func(i, j int) bool {
			if transfers[i].BlockNumber == transfers[j].BlockNumber {
				if transfers[i].TransactionIndex == transfers[j].TransactionIndex {
					return transfers[i].LogIndex < transfers[j].LogIndex
				}
				return transfers[i].TransactionIndex < transfers[j].TransactionIndex
			}
			return transfers[i].BlockNumber < transfers[j].BlockNumber
		})
		for _, t := range transfers {
			logger.TestLog(true, "transfer:", t.BlockNumber, t.TransactionIndex, t.LogIndex, t.Asset, t.Holder, t.Amount.Text(10), t.CorrectingReason)
		}
	}

	return transfers, nil
}
