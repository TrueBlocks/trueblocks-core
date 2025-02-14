package ledger2

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func TestNewLedgerEntry(t *testing.T) {
	le := NewLedgerEntry("test-appearance", 12345, 2)
	if le.AppearanceID != "test-appearance" {
		t.Fatalf("AppearanceID mismatch. got=%s want=%s", le.AppearanceID, "test-appearance")
	}
	if le.BlockNumber != 12345 {
		t.Fatalf("BlockNumber mismatch. got=%d want=%d", le.BlockNumber, 12345)
	}
	if le.TransactionIndex != 2 {
		t.Fatalf("TransactionIndex mismatch. got=%d want=%d", le.TransactionIndex, 2)
	}
	if len(le.Postings) != 0 {
		t.Fatalf("Expected no postings initially. got=%d want=0", len(le.Postings))
	}
}

func TestAppendPosting(t *testing.T) {
	le := NewLedgerEntry("some-app", 100, 0)

	p1 := NewPosting(100, 0, "log_1", 999)
	p1.TransferIn = *base.NewWei(50)
	p1.TransferOut = *base.NewWei(10)
	le.AppendPosting(p1)

	if len(le.Postings) != 1 {
		t.Fatalf("Expected 1 posting after append. got=%d", len(le.Postings))
	}

	p2 := NewPosting(100, 0, "log_2", 999)
	p2.TransferIn = *base.NewWei(20)
	p2.GasOut = *base.NewWei(5)
	le.AppendPosting(p2)

	if len(le.Postings) != 2 {
		t.Fatalf("Expected 2 postings after second append. got=%d", len(le.Postings))
	}
}

func TestLedgerEntryAggregation(t *testing.T) {
	le := NewLedgerEntry("agg-app", 200, 5)

	// Posting 1: In=80, Out=10
	p1 := NewPosting(200, 5, "log_1", 123)
	p1.TransferIn = *base.NewWei(80)
	p1.TransferOut = *base.NewWei(10)
	le.AppendPosting(p1)

	// Posting 2: In=25, Out=5
	p2 := NewPosting(200, 5, "log_2", 123)
	p2.TransferIn = *base.NewWei(25)
	p2.GasOut = *base.NewWei(5)
	le.AppendPosting(p2)

	totalIn := le.TotalIn()
	if totalIn.String() != "105" {
		t.Fatalf("TotalIn mismatch. got=%s want=105", totalIn.String())
	}

	totalOut := le.TotalOut()
	if totalOut.String() != "15" {
		t.Fatalf("TotalOut mismatch. got=%s want=15", totalOut.String())
	}

	netVal := le.NetValue()
	if netVal.String() != "90" {
		t.Fatalf("NetValue mismatch. got=%s want=90", netVal.String())
	}
}

func TestLedgerEntryString(t *testing.T) {
	le := NewLedgerEntry("str-app", 300, 2)

	p := NewPosting(300, 2, "abc", 9999)
	p.TransferIn = *base.NewWei(100)
	p.TransferOut = *base.NewWei(60)
	le.AppendPosting(p)

	got := le.String()
	want := "LedgerEntry(AppearanceID=str-app Block=300 Tx=2 Postings=1 In=100 Out=60 Net=40)"
	if got != want {
		t.Fatalf("String mismatch.\ngot:  %s\nwant: %s", got, want)
	}
}
