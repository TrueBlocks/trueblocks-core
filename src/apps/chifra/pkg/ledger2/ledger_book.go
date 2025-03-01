package ledger2

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// LedgerBook holds multiple Ledger items for a single AccountedFor.
type LedgerBook struct {
	AccountedFor base.Address
	Ledgers      map[string]Ledger
}

// NewLedgerBook creates a LedgerBook for a given address, with no Ledgers initially.
func NewLedgerBook(accountedForAddress base.Address) LedgerBook {
	return LedgerBook{
		AccountedFor: accountedForAddress,
		Ledgers:      make(map[string]Ledger),
	}
}

// String returns a concise description of this LedgerBook, including how many Ledgers it contains.
func (lb *LedgerBook) String() string {
	totIn := lb.TotalIn()
	totOut := lb.TotalOut()
	net := lb.NetValue()
	return fmt.Sprintf(
		"%s %d %s %s %s)",
		lb.AccountedFor,
		len(lb.Ledgers),
		totIn.String(),
		totOut.String(),
		net.String(),
	)
}

// GetLedger retrieves the Ledger for a given asset address, if it exists.
func (lb *LedgerBook) GetLedger(assetAddress base.Address) (Ledger, bool) {
	l, ok := lb.Ledgers[assetAddress.Hex()]
	return l, ok
}

// TotalIn returns the sum of total incoming amounts across all Ledgers in this LedgerBook.
func (lb *LedgerBook) TotalIn() base.Wei {
	total := base.NewWei(0)
	for _, l := range lb.Ledgers {
		sum := l.TotalIn()
		total = new(base.Wei).Add(total, &sum)
	}
	return *total
}

// TotalOut returns the sum of total outgoing amounts across all Ledgers in this LedgerBook.
func (lb *LedgerBook) TotalOut() base.Wei {
	total := base.NewWei(0)
	for _, l := range lb.Ledgers {
		sum := l.TotalOut()
		total = new(base.Wei).Add(total, &sum)
	}
	return *total
}

// NetValue returns the difference between total incoming and total outgoing across all Ledgers.
func (lb *LedgerBook) NetValue() base.Wei {
	in := lb.TotalIn()
	out := lb.TotalOut()
	net := new(base.Wei).Sub(&in, &out)
	return *net
}

// Statements returns a slice of types.Statement, one for each Posting in the LedgerBook.
// It copies fields from the Posting and its parent Ledger and LedgerEntry as appropriate.
// Unused fields in Statement are left at their default zero values.
func (lb *LedgerBook) Statements() ([]types.Statement, error) {
	var stmts []types.Statement

	for _, l := range lb.Ledgers {
		for _, entry := range l.Entries {
			for _, posting := range entry.Postings {
				s := posting.Statement
				s.AccountedFor = lb.AccountedFor
				stmts = append(stmts, s)
			}
		}
	}

	return stmts, nil
}
