package ledger2

import (
	"encoding/json"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// LedgerEntry represents a collection of Posting items that derive from a single appearance.
type LedgerEntry struct {
	AppearanceID     string
	BlockNumber      base.Blknum
	TransactionIndex base.Txnum
	Postings         []Posting
}

// NewLedgerEntry creates a LedgerEntry from the given appearance data, returning by value.
func NewLedgerEntry(appearanceID string, blockNum base.Blknum, txIndex base.Txnum) LedgerEntry {
	return LedgerEntry{
		AppearanceID:     appearanceID,
		BlockNumber:      blockNum,
		TransactionIndex: txIndex,
		Postings:         make([]Posting, 0),
	}
}

// String returns a human-readable summary of the LedgerEntry.
func (le *LedgerEntry) String() string {
	bytes, _ := json.Marshal(le)
	return string(bytes)
}

// TotalIn sums the total incoming values for all Postings in this LedgerEntry.
func (le *LedgerEntry) TotalIn() base.Wei {
	total := base.ZeroWei
	for _, posting := range le.Postings {
		tmp := posting.TotalIn()
		total = total.Add(total, tmp)
	}
	return *total
}

// TotalOut sums the total outgoing values for all Postings in this LedgerEntry.
func (le *LedgerEntry) TotalOut() base.Wei {
	total := base.ZeroWei
	for _, posting := range le.Postings {
		tmp := posting.TotalOut()
		total = total.Add(total, tmp)
	}
	return *total
}

// NetValue is the difference between total incoming and total outgoing in this LedgerEntry.
func (le *LedgerEntry) NetValue() base.Wei {
	in := le.TotalIn()
	out := le.TotalOut()
	net := base.ZeroWei
	net = net.Sub(&in, &out)
	return *net
}
