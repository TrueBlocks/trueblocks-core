package ledger2

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (r *Reconciler) GetStatements(prev, next base.Blknum, filter *filter.AppearanceFilter, trans *types.Transaction) ([]types.Statement, error) {
	apps := []types.Appearance{
		{Address: r.LedgerBook.AccountedFor, BlockNumber: uint32(trans.BlockNumber), TransactionIndex: uint32(trans.TransactionIndex)},
	}
	xfers := DeriveAssetTransfers(r.LedgerBook.AccountedFor, trans)
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
					AccountedFor:       base.Address(lb.AccountedFor),
					AssetAddress:       base.Address(l.AssetAddress),
					AssetSymbol:        l.AssetName,
					BlockNumber:        posting.BlockNumber,
					TransactionIndex:   posting.TransactionIndex,
					Timestamp:          posting.Timestamp,
					AmountIn:           posting.TransferIn,
					AmountOut:          posting.TransferOut,
					GasOut:             posting.GasOut,
					InternalOut:        posting.InternalTxFeesOut,
					PrefundIn:          posting.PrefundIn,
					SelfDestructIn:     posting.SelfDestructIn,
					CorrectingIn:       posting.CorrectingIn,
					CorrectingOut:      posting.CorrectingOut,
					CorrectingReason:   posting.CorrectingReason,
					MinerBaseRewardIn:  posting.MiningRewardIn,
					MinerUncleRewardIn: posting.UncleRewardIn,
					RollingBalance:     posting.RunningBalance,
					Sender:             posting.From,
					Recipient:          posting.To,
					// TODO: BOGUS SHIT!
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
