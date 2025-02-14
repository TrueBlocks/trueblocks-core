package ledger2

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func GetStatements(accountFor base.Address, filter *filter.AppearanceFilter, trans *types.Transaction) ([]types.Statement, error) {
	r := NewReconciler(accountFor)
	apps := []types.Appearance{
		{Address: accountFor, BlockNumber: uint32(trans.BlockNumber), TransactionIndex: uint32(trans.TransactionIndex)},
	}

	xfers := DeriveAssetTransfers(accountFor, trans)
	r.ProcessAppearances(apps, xfers)

	return r.LedgerBook.Statements()
}

// Statements returns a slice of types.Statement, one for each Posting in the LedgerBook.
// It copies fields from the Posting and its parent Ledger and LedgerEntry as appropriate.
// Unused fields in Statement are left at their default zero values.
func (lb *LedgerBook) Statements() ([]types.Statement, error) {
	var stmts []types.Statement

	for _, l := range lb.Ledgers {
		for _, entry := range l.Entries {
			for _, posting := range entry.Postings {
				s := types.Statement{
					// Mapped from LedgerBook
					AccountedFor: base.Address(lb.AccountedForAddress),

					// Mapped from Ledger (asset info)
					AssetAddr:   base.Address(l.AssetAddress),
					AssetSymbol: l.AssetName,

					// Mapped from Posting
					BlockNumber:      posting.BlockNumber,
					TransactionIndex: posting.TransactionIndex,
					Timestamp:        posting.Timestamp,

					// Simple incoming/outgoing assignments
					AmountIn:  posting.TransferIn,
					AmountOut: posting.TransferOut,

					// Fees and special movement types
					GasOut:             posting.GasOut,
					InternalOut:        posting.InternalTxFeesOut,
					PrefundIn:          posting.PrefundIn,
					SelfDestructIn:     posting.SelfDestructIn,
					CorrectingIn:       posting.CorrectingIn,
					CorrectingOut:      posting.CorrectingOut,
					CorrectingReason:   posting.CorrectingReason,
					MinerBaseRewardIn:  posting.MiningRewardIn,
					MinerUncleRewardIn: posting.UncleRewardIn,

					// Rolling balance
					RollingBalance: posting.RunningBalance,

					// Some fields in Statement (like Sender, Recipient, TransactionHash, etc.)
					// are not easily mapped from Posting or Ledger data, so they are left at default zero/empty.
					// Similarly, BegBal, EndBal, SpotPrice, and so on remain at default values.
				}

				// You can further customize or derive additional fields here as needed.

				stmts = append(stmts, s)
			}
		}
	}

	return stmts, nil
}
