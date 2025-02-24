package ledger2

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func TestNewLedger(t *testing.T) {
	l := NewLedger(base.HexToAddress("0xABC"))
	if l.AssetAddress != base.HexToAddress("0xABC") {
		t.Fatalf("AssetAddress mismatch. got=%s want=%s", l.AssetAddress, "0xABC")
	}
	if len(l.Entries) != 0 {
		t.Fatalf("Expected no entries initially. got=%d want=0", len(l.Entries))
	}
}

func TestAppendEntry(t *testing.T) {
	l := NewLedger(base.HexToAddress("0xAAA"))

	e1 := NewLedgerEntry("appearance-1", 100, 0)
	l.Entries = append(l.Entries, e1)
	if len(l.Entries) != 1 {
		t.Fatalf("Expected 1 entry. got=%d want=1", len(l.Entries))
	}

	e2 := NewLedgerEntry("appearance-2", 101, 1)
	l.Entries = append(l.Entries, e2)
	if len(l.Entries) != 2 {
		t.Fatalf("Expected 2 entries. got=%d want=2", len(l.Entries))
	}
}

func TestLedgerAggregation(t *testing.T) {
	l := NewLedger(base.HexToAddress("0xDEF"))

	// Entry 1: total in = 80, total out = 10
	e1 := NewLedgerEntry("appearance-1", 200, 5)
	p1 := types.NewPosting(200, 5, 1, 9999, base.ZeroAddr)
	p1.AmountIn = *base.NewWei(80)
	p1.AmountOut = *base.NewWei(10)
	e1.Postings = append(e1.Postings, p1)
	l.Entries = append(l.Entries, e1)

	// Entry 2: total in = 25, total out = 5
	e2 := NewLedgerEntry("appearance-2", 201, 3)
	p2 := types.NewPosting(201, 3, 2, 9999, base.ZeroAddr)
	p2.AmountIn = *base.NewWei(25)
	p2.GasOut = *base.NewWei(5)
	e2.Postings = append(e2.Postings, p2)
	l.Entries = append(l.Entries, e2)

	in := l.TotalIn()
	if in.String() != "105" {
		t.Fatalf("TotalIn mismatch. got=%s want=105", in.String())
	}

	out := l.TotalOut()
	if out.String() != "15" {
		t.Fatalf("TotalOut mismatch. got=%s want=15", out.String())
	}

	netVal := l.NetValue()
	if netVal.String() != "90" {
		t.Fatalf("NetValue mismatch. got=%s want=90", netVal.String())
	}
}

func TestLedgerString(t *testing.T) {
	l := NewLedger(base.HexToAddress("0xFED"))

	e := NewLedgerEntry("some-app", 300, 2)
	p := types.NewPosting(300, 2, 12, 2, base.ZeroAddr)
	p.AmountIn = *base.NewWei(100)
	p.AmountOut = *base.NewWei(60)
	e.Postings = append(e.Postings, p)
	l.Entries = append(l.Entries, e)

	got := l.String()
	want := "Ledger(AssetAddress=0x0000000000000000000000000000000000000fed Entries=1 In=100 Out=60 Net=40)"
	if got != want {
		t.Fatalf("String mismatch.\ngot:  %s\nwant: %s", got, want)
	}
}
