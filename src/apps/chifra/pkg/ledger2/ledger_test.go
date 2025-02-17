package ledger2

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func TestNewLedger(t *testing.T) {
	l := NewLedger(base.HexToAddress("0xABC"), "TEST")
	if l.AssetAddress != base.HexToAddress("0xABC") {
		t.Fatalf("AssetAddress mismatch. got=%s want=%s", l.AssetAddress, "0xABC")
	}
	if l.AssetName != "TEST" {
		t.Fatalf("AssetName mismatch. got=%s want=%s", l.AssetName, "TEST")
	}
	if len(l.Entries) != 0 {
		t.Fatalf("Expected no entries initially. got=%d want=0", len(l.Entries))
	}
}

func TestAppendEntry(t *testing.T) {
	l := NewLedger(base.HexToAddress("0xAAA"), "AAA")

	e1 := NewLedgerEntry("appearance-1", 100, 0)
	l.AppendEntry(e1)
	if len(l.Entries) != 1 {
		t.Fatalf("Expected 1 entry. got=%d want=1", len(l.Entries))
	}

	e2 := NewLedgerEntry("appearance-2", 101, 1)
	l.AppendEntry(e2)
	if len(l.Entries) != 2 {
		t.Fatalf("Expected 2 entries. got=%d want=2", len(l.Entries))
	}
}

func TestLedgerAggregation(t *testing.T) {
	l := NewLedger(base.HexToAddress("0xDEF"), "DEF")

	// Entry 1: total in = 80, total out = 10
	e1 := NewLedgerEntry("appearance-1", 200, 5)
	p1 := NewPosting(200, 5, "log_1", 9999)
	p1.TransferIn = *base.NewWei(80)
	p1.TransferOut = *base.NewWei(10)
	e1.AppendPosting(p1)
	l.AppendEntry(e1)

	// Entry 2: total in = 25, total out = 5
	e2 := NewLedgerEntry("appearance-2", 201, 3)
	p2 := NewPosting(201, 3, "log_2", 9999)
	p2.TransferIn = *base.NewWei(25)
	p2.GasOut = *base.NewWei(5)
	e2.AppendPosting(p2)
	l.AppendEntry(e2)

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

// func TestLedgerString(t *testing.T) {
// 	l := NewLedger(base.HexToAddress("0xFED"), "FED")

// 	e := NewLedgerEntry("some-app", 300, 2)
// 	p := NewPosting(300, 2, "abc", 111)
// 	p.TransferIn = *base.NewWei(100)
// 	p.TransferOut = *base.NewWei(60)
// 	e.AppendPosting(p)

// 	l.AppendEntry(e)

// 	got := l.String()
// 	// We expect: Ledger(AssetAddress=0xLED AssetName=LED Entries=1 In=100 Out=60 Net=40)
// 	want := "Ledger(AssetAddress=0xLED AssetName=LED Entries=1 In=100 Out=60 Net=40)"
// 	if got != want {
// 		t.Fatalf("String mismatch.\ngot:  %s\nwant: %s", got, want)
// 	}
// }
