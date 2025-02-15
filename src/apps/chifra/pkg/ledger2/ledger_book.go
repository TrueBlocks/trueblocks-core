package ledger2

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
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
		"LedgerBook(Address=%s Ledgers=%d In=%s Out=%s Net=%s)",
		lb.AccountedFor,
		len(lb.Ledgers),
		totIn.String(),
		totOut.String(),
		net.String(),
	)
}

// AddLedger inserts a new Ledger into this LedgerBook, keyed by its AssetAddr.
func (lb *LedgerBook) AddLedger(l Ledger) {
	lb.Ledgers[l.AssetAddr.Hex()] = l
}

// GetLedger retrieves the Ledger for a given asset address, if it exists.
func (lb *LedgerBook) GetLedger(assetAddress string) (Ledger, bool) {
	l, ok := lb.Ledgers[assetAddress]
	return l, ok
}

// TotalIn returns the sum of total incoming amounts across all Ledgers in this LedgerBook.
func (lb *LedgerBook) TotalIn() base.Wei {
	total := base.NewWei(0)
	for _, l := range lb.Ledgers {
		sum := l.TotalIn()
		total = total.Add(total, &sum)
	}
	return *total
}

// TotalOut returns the sum of total outgoing amounts across all Ledgers in this LedgerBook.
func (lb *LedgerBook) TotalOut() base.Wei {
	total := base.NewWei(0)
	for _, l := range lb.Ledgers {
		sum := l.TotalOut()
		total = total.Add(total, &sum)
	}
	return *total
}

// NetValue returns the difference between total incoming and total outgoing across all Ledgers.
func (lb *LedgerBook) NetValue() base.Wei {
	in := lb.TotalIn()
	out := lb.TotalOut()
	net := base.NewWei(0)
	net = net.Sub(&in, &out)
	return *net
}
