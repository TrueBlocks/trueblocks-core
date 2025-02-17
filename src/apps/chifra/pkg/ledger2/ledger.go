package ledger2

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// Ledger is a collection of LedgerEntry items that refer to a single asset.
type Ledger struct {
	AssetAddress base.Address
	Entries      []LedgerEntry
}

// NewLedger creates a Ledger with the given asset address and asset name.
func NewLedger(assetAddress base.Address) Ledger {
	return Ledger{
		AssetAddress: assetAddress,
		Entries:      make([]LedgerEntry, 0),
	}
}

// String returns a human-readable summary of the Ledger.
func (l *Ledger) String() string {
	totIn := l.TotalIn()
	totOut := l.TotalOut()
	net := l.NetValue()
	return fmt.Sprintf(
		"Ledger(AssetAddress=%s Entries=%d In=%s Out=%s Net=%s)",
		l.AssetAddress,
		len(l.Entries),
		totIn.String(),
		totOut.String(),
		net.String(),
	)
}

// TotalIn sums the total incoming values for all LedgerEntry items in this Ledger.
func (l *Ledger) TotalIn() base.Wei {
	total := base.NewWei(0)
	for _, entry := range l.Entries {
		entryIn := entry.TotalIn()
		total = total.Add(total, &entryIn)
	}
	return *total
}

// TotalOut sums the total outgoing values for all LedgerEntry items in this Ledger.
func (l *Ledger) TotalOut() base.Wei {
	total := base.NewWei(0)
	for _, entry := range l.Entries {
		entryOut := entry.TotalOut()
		total = total.Add(total, &entryOut)
	}
	return *total
}

// NetValue is the difference between total incoming and total outgoing across the ledger.
func (l *Ledger) NetValue() base.Wei {
	in := l.TotalIn()
	out := l.TotalOut()
	net := base.NewWei(0)
	net = net.Sub(&in, &out)
	return *net
}
