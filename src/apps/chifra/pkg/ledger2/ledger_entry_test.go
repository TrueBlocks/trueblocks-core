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

	p1 := Posting{
		BlockNumber:      100,
		TransactionIndex: 0,
		LogIndex:         1,
		Timestamp:        999,
		AmountIn:         *base.NewWei(50),
		AmountOut:        *base.NewWei(10),
	}
	le.Postings = append(le.Postings, p1)

	if len(le.Postings) != 1 {
		t.Fatalf("Expected 1 posting after append. got=%d", len(le.Postings))
	}

	p2 := Posting{
		BlockNumber:      100,
		TransactionIndex: 0,
		LogIndex:         2,
		Timestamp:        999,
		AmountIn:         *base.NewWei(20),
		GasOut:           *base.NewWei(5),
	}
	le.Postings = append(le.Postings, p2)

	if len(le.Postings) != 2 {
		t.Fatalf("Expected 2 postings after second append. got=%d", len(le.Postings))
	}
}

func TestLedgerEntryAggregation(t *testing.T) {
	le := NewLedgerEntry("agg-app", 200, 5)

	// Posting 1: In=80, Out=10
	p1 := Posting{
		BlockNumber:      200,
		TransactionIndex: 5,
		LogIndex:         1,
		Timestamp:        123,
		AmountIn:         *base.NewWei(80),
		AmountOut:        *base.NewWei(10),
	}
	le.Postings = append(le.Postings, p1)

	// Posting 2: In=25, Out=5
	p2 := Posting{
		BlockNumber:      200,
		TransactionIndex: 5,
		LogIndex:         2,
		Timestamp:        123,
		AmountIn:         *base.NewWei(25),
		GasOut:           *base.NewWei(5),
	}
	le.Postings = append(le.Postings, p2)

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

	p := Posting{
		BlockNumber:      300,
		TransactionIndex: 2,
		LogIndex:         1,
		Timestamp:        9999,
		AmountIn:         *base.NewWei(100),
		AmountOut:        *base.NewWei(60),
	}
	le.Postings = append(le.Postings, p)

	got := le.String()
	want := "LedgerEntry(AppearanceID=str-app Block=300 Tx=2 Postings=1 In=100 Out=60 Net=40)"
	if got != want {
		t.Fatalf("String mismatch.\ngot:  %s\nwant: %s", got, want)
	}
}
