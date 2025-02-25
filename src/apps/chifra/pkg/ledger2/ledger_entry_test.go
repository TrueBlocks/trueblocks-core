package ledger2

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
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

	p1 := types.NewPosting(100, 0, 1, 999)
	p1.AmountIn = *base.NewWei(50)
	p1.AmountOut = *base.NewWei(10)
	le.Postings = append(le.Postings, p1)

	if len(le.Postings) != 1 {
		t.Fatalf("Expected 1 posting after append. got=%d", len(le.Postings))
	}

	p2 := types.NewPosting(100, 0, 2, 999)
	p2.AmountIn = *base.NewWei(20)
	p2.GasOut = *base.NewWei(5)
	le.Postings = append(le.Postings, p2)

	if len(le.Postings) != 2 {
		t.Fatalf("Expected 2 postings after second append. got=%d", len(le.Postings))
	}
}

func TestLedgerEntryAggregation(t *testing.T) {
	le := NewLedgerEntry("agg-app", 200, 5)

	// Posting 1: In=80, Out=10
	p1 := types.NewPosting(200, 5, 1, 123)
	p1.AmountIn = *base.NewWei(80)
	p1.AmountOut = *base.NewWei(10)
	le.Postings = append(le.Postings, p1)

	// Posting 2: In=25, Out=5
	p2 := types.NewPosting(200, 5, 2, 123)
	p2.AmountIn = *base.NewWei(25)
	p2.GasOut = *base.NewWei(5)
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

	p := types.NewPosting(300, 2, 1, 9999)
	p.AmountIn = *base.NewWei(100)
	p.AmountOut = *base.NewWei(60)
	le.Postings = append(le.Postings, p)

	got := le.String()
	want := `{"AppearanceID":"str-app","BlockNumber":300,"TransactionIndex":2,"Postings":[{"accountedFor":"0x0","amountIn":"0","amountOut":"0","assetAddress":"0x0","assetSymbol":"","begBal":"0","blockNumber":300,"blockNumberNext":0,"blockNumberPrev":0,"correctingIn":"0","correctingOut":"0","decimals":0,"endBal":"0","gasOut":"0","internalIn":"0","internalOut":"0","logIndex":1,"minerBaseRewardIn":"0","minerNephewRewardIn":"0","minerTxFeeIn":"0","minerUncleRewardIn":"0","prefundIn":"0","prevBal":"0","priceSource":"","recipient":"0x0","rollingBalance":"0","selfDestructIn":"0","selfDestructOut":"0","sender":"0x0","spotPrice":0,"timestamp":9999,"transactionHash":"0x0000000000000000000000000000000000000000000000000000000000000000","transactionIndex":2}]}`
	if got != want {
		t.Fatalf("String mismatch.\ngot:  %s\nwant: %s", got, want)
	}
}
