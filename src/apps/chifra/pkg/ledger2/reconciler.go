package ledger2

import (
	"encoding/json"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger10"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type Reconciler2 struct {
	opts              *ledger10.ReconcilerOptions
	names             map[base.Address]types.Name
	hasStartBlock     bool
	transfers         map[blockTxKey][]types.Statement
	accountLedger     map[assetHolderKey]base.Wei
	ledgerAssets      map[base.Address]bool
	correctionCounter base.Value
	entryCounter      base.Value
	ledgers           map[base.Address]Ledger
}

func (r *Reconciler2) String() string {
	bytes, _ := json.MarshalIndent(r, "", "  ")
	return string(bytes)
}

func NewReconciler(opts *ledger10.ReconcilerOptions) *Reconciler2 {
	parts := types.Custom | types.Prefund | types.Regular
	names, _ := names.LoadNamesMap(opts.Connection.Chain, parts, []string{})
	r := &Reconciler2{
		opts:          opts,
		names:         names,
		hasStartBlock: false,
		transfers:     make(map[blockTxKey][]types.Statement),
		accountLedger: make(map[assetHolderKey]base.Wei),
		ledgerAssets:  make(map[base.Address]bool),
		ledgers:       make(map[base.Address]Ledger),
	}
	_ = r.correctionCounter
	_ = r.entryCounter
	return r
}

type blockTxKey struct {
	BlockNumber      base.Blknum
	TransactionIndex base.Txnum
}

type assetHolderKey struct {
	Asset  base.Address
	Holder base.Address
}

func (r *Reconciler2) GetLedger(asset base.Address) (Ledger, bool) {
	l, ok := r.ledgers[asset]
	return l, ok
}

func (r *Reconciler2) Statements() ([]types.Statement, error) {
	var stmts []types.Statement

	for _, l := range r.ledgers {
		for _, entry := range l.Entries {
			for _, posting := range entry.Postings {
				s := posting.Statement
				stmts = append(stmts, s)
			}
		}
	}

	return stmts, nil
}

type LedgerEntry struct {
	AppearanceID     string
	BlockNumber      base.Blknum
	TransactionIndex base.Txnum
	Postings         []types.Posting
}

func (le *LedgerEntry) String() string {
	bytes, _ := json.Marshal(le)
	return string(bytes)
}

func (le *LedgerEntry) TotalIn() base.Wei {
	total := base.ZeroWei
	for _, posting := range le.Postings {
		tmp := posting.TotalIn()
		total = new(base.Wei).Add(total, tmp)
	}
	return *total
}

func (le *LedgerEntry) TotalOut() base.Wei {
	total := base.ZeroWei
	for _, posting := range le.Postings {
		tmp := posting.TotalOut()
		total = new(base.Wei).Add(total, tmp)
	}
	return *total
}

func (le *LedgerEntry) NetValue() base.Wei {
	in := le.TotalIn()
	out := le.TotalOut()
	net := new(base.Wei).Sub(&in, &out)
	return *net
}

type Ledger struct {
	Asset   base.Address
	Entries []LedgerEntry
}

func NewLedger(asset base.Address) Ledger {
	return Ledger{
		Asset:   asset,
		Entries: make([]LedgerEntry, 0),
	}
}

func (l *Ledger) String() string {
	totIn := l.TotalIn()
	totOut := l.TotalOut()
	net := l.NetValue()
	return fmt.Sprintf(
		"Ledger(Asset=%s Entries=%d In=%s Out=%s Net=%s)",
		l.Asset,
		len(l.Entries),
		totIn.String(),
		totOut.String(),
		net.String(),
	)
}

func (l *Ledger) TotalIn() base.Wei {
	total := base.NewWei(0)
	for _, entry := range l.Entries {
		entryIn := entry.TotalIn()
		total = new(base.Wei).Add(total, &entryIn)
	}
	return *total
}

func (l *Ledger) TotalOut() base.Wei {
	total := base.NewWei(0)
	for _, entry := range l.Entries {
		entryOut := entry.TotalOut()
		total = new(base.Wei).Add(total, &entryOut)
	}
	return *total
}

func (l *Ledger) NetValue() base.Wei {
	in := l.TotalIn()
	out := l.TotalOut()
	net := new(base.Wei).Sub(&in, &out)
	return *net
}
