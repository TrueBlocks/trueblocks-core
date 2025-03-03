package ledger2

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger3"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pricing"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// Reconciler2 is responsible for processing Appearances, constructing Postings
// and LedgerEntries, appending them to Ledgers, and maintaining a LedgerBook.
type Reconciler2 struct {
	LedgerBook LedgerBook
	// mu         sync.Mutex
	accountFor base.Address
	// firstBlock  base.Blknum
	// lastBlock   base.Blknum
	names map[base.Address]types.Name
	// testMode    bool
	asEther bool
	// reversed    bool
	// useTraces   bool
	connection  *rpc.Connection
	assetFilter []base.Address
}

// NewReconciler2 creates a Reconciler2 for the specified accountedForAddress.
func NewReconciler2(conn *rpc.Connection, assetFilters *[]string, accountedForAddress base.Address, asEth bool) Reconciler2 {
	parts := types.Custom | types.Prefund | types.Regular
	names, _ := names.LoadNamesMap(conn.Chain, parts, []string{})
	ret := Reconciler2{
		LedgerBook: NewLedgerBook(accountedForAddress),
		accountFor: accountedForAddress,
		connection: conn,
		names:      names,
		asEther:    asEth,
	}
	if assetFilters != nil {
		ret.assetFilter = make([]base.Address, len(*assetFilters))
		for i, addr := range *assetFilters {
			ret.assetFilter[i] = base.HexToAddress(addr)
		}
	} else {
		ret.assetFilter = []base.Address{}
	}
	return ret
}

// String returns a summary of the Reconciler2’s LedgerBook.
func (r *Reconciler2) String() string {
	return r.LedgerBook.String()
}

func (r *Reconciler2) GetStatements(pos *types.AppPosition, filter *filter.AppearanceFilter, trans *types.Transaction) ([]types.Statement, error) {
	if transfers, err := r.GetAssetTransfers(pos, filter, trans); err != nil {
		return []types.Statement{}, err
	} else {
		r.ProcessTransaction(pos, trans, transfers)
		if !r.LedgerBook.IsMaterial() {
			return []types.Statement{}, nil
		}
		return r.LedgerBook.Statements()
	}
}

func (lb *LedgerBook) IsMaterial() bool {
	if len(lb.Ledgers) == 0 {
		return false
	}

	for _, l := range lb.Ledgers {
		net := l.NetValue()
		if !net.Equal(base.ZeroWei) {
			return true
		}
	}

	return false
}

// ProcessTransaction takes a list of Appearances and their related AssetTransfers,
// converts them to Postings, and appends them into the appropriate Ledger.
func (r *Reconciler2) ProcessTransaction(pos *types.AppPosition, trans *types.Transaction, allTransfers []ledger3.AssetTransfer) {
	// We assume allTransfers includes every relevant ledger3.AssetTransfer for the Appearances.
	// In reality, you'd fetch them from an indexer or node calls.
	//
	// We'll map an appearanceID => LedgerEntry as we go along, then store them in the
	// correct Ledger for the asset once we've built them fully.
	//
	// This is a simplistic approach; a production system might have concurrency, etc.

	// For quick lookups:
	appearanceByID := make(map[string]types.Appearance)
	app := types.Appearance{Address: r.LedgerBook.AccountedFor, BlockNumber: uint32(trans.BlockNumber), TransactionIndex: uint32(trans.TransactionIndex)}
	appID := fmt.Sprintf("%d-%d", app.BlockNumber, app.TransactionIndex)
	appearanceByID[appID] = app

	// Build ledger entries keyed by (assetAddress + appearanceID)
	// because we might have multiple assets in the same transaction appearance
	// (e.g., an ERC20 plus some ETH).
	entriesMap := make(map[string]*LedgerEntry)

	for _, at := range allTransfers {
		// Find which appearance it belongs to
		appID := fmt.Sprintf("%d-%d", at.BlockNumber, at.TransactionIndex)
		app, ok := appearanceByID[appID]
		if !ok {
			// Possibly this is not relevant to the Reconciler's address or is missing
			continue
		}

		// Build a unique key per asset + app
		key := fmt.Sprintf("%s|%s", at.AssetAddress, appID)

		// Create the LedgerEntry if needed
		entry, found := entriesMap[key]
		if !found {
			newEntry := NewLedgerEntry(appID, base.Blknum(app.BlockNumber), base.Txnum(app.TransactionIndex))
			entriesMap[key] = &newEntry
			entry = &newEntry
		}

		// Convert the ledger3.AssetTransfer into a single Posting (some logic is simplified):
		posting := r.queryBalances(pos, at)

		// 	var okay bool
		// 	if okay = s.Reconciled(); !okay {
		// 		if okay = l.CorrectForNullTransfer(s, trans); !okay {
		// 			if !s.IsEth() {
		// 				_ = l.correctForSomethingElseToken(s)
		// 			}
		// 		}
		// 	}

		if posting.IsMaterial() {
			posting.SpotPrice, posting.PriceSource, _ = pricing.PriceUsd(r.connection, (*types.Statement)(&posting.Statement))
		}

		if file.IsTestMode() {
			(*types.Statement)(&posting.Statement).DebugStatement(pos)
		}

		entry.Postings = append(entry.Postings, posting)
	}

	// Now we have a set of ledger entries. We'll append them into the correct Ledger for each asset.
	for key, lePtr := range entriesMap {
		// key is "assetAddress|block-tx"
		assetAddress := key[:findSeparator(key)]
		ledger, found := r.LedgerBook.GetLedger(base.HexToAddress(assetAddress))
		if !found {
			ledger = NewLedger(base.HexToAddress(assetAddress))
		}
		ledger.Entries = append(ledger.Entries, *lePtr)
		r.LedgerBook.Ledgers[ledger.AssetAddress.Hex()] = ledger
	}
}

func CorrectForNullTransfer(s *types.Statement, tx *types.Transaction) bool {
	if s.IsNullTransfer(tx) {
		logger.TestLog(true, "Correcting token transfer for a null transfer")
		amt := s.TotalIn() // use totalIn since this is the amount that was faked
		s.AmountOut = *base.ZeroWei
		s.AmountIn = *base.ZeroWei
		s.CorrectingIn = *amt
		s.CorrectingOut = *amt
		s.CorrectingReason = "null-transfer"
	} else {
		logger.TestLog(true, "Needs correction for token transfer")
	}

	return s.Reconciled()
}

// queryBalances transforms a single ledger3.AssetTransfer into a basic Posting.
func (r *Reconciler2) queryBalances(pos *types.AppPosition, at ledger3.AssetTransfer) types.Posting {
	ret := types.Posting{}
	ret.Statement = at

	if at.PostAssetType != types.TrialBalToken && at.PostAssetType != types.TrialBalNft {
		prevBal, _ := r.connection.GetBalanceAt(r.LedgerBook.AccountedFor, pos.Prev)
		if at.BlockNumber == 0 {
			prevBal = new(base.Wei)
		}

		begBal, _ := r.connection.GetBalanceAt(r.LedgerBook.AccountedFor, at.BlockNumber-1)
		if at.BlockNumber == 0 {
			begBal = new(base.Wei)
		}

		endBal, _ := r.connection.GetBalanceAt(r.LedgerBook.AccountedFor, at.BlockNumber)

		ret.PrevBal = *prevBal
		ret.BegBal = *begBal
		ret.EndBal = *endBal

	} else {
		prevBal, _ := r.connection.GetBalanceAtToken(at.AssetAddress, r.LedgerBook.AccountedFor, pos.Prev)
		if at.BlockNumber == 0 {
			prevBal = new(base.Wei)
		}

		begBal, _ := r.connection.GetBalanceAtToken(at.AssetAddress, r.LedgerBook.AccountedFor, at.BlockNumber-1)
		if at.BlockNumber == 0 {
			begBal = new(base.Wei)
		}
		endBal, _ := r.connection.GetBalanceAtToken(at.AssetAddress, r.LedgerBook.AccountedFor, at.BlockNumber)
		ret.PrevBal = *prevBal
		ret.BegBal = *begBal
		ret.EndBal = *endBal
	}
	return ret
}

// ReconcileCheckpoint is a placeholder for a method that verifies final ledger balances
// at a given block boundary. Real logic might compare on-chain balances and create
// correcting postings if there's a mismatch.
func (r *Reconciler2) ReconcileCheckpoint(block base.Blknum) {
	// Example placeholder
	_ = block
}

// findSeparator is a small helper to locate the '|' in the string.
func findSeparator(s string) int {
	for i := 0; i < len(s); i++ {
		if s[i] == '|' {
			return i
		}
	}
	return len(s)
}
