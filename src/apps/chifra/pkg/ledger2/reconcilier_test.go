package ledger2

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// TestReconcilerBasic runs a simple scenario where the accountedForAddress is "0x1002"
// and we pass in one or two appearances plus some asset transfers.
func TestReconcilerBasic(t *testing.T) {
	// Suppose this is our address of interest
	rec := NewReconciler(base.HexToAddress("0x1002"))

	appearances := []types.Appearance{
		{Address: base.HexToAddress("0x1002"), BlockNumber: 123, TransactionIndex: 0},
		{Address: base.HexToAddress("0x1002"), BlockNumber: 123, TransactionIndex: 1},
		{Address: base.HexToAddress("0x1002"), BlockNumber: 124, TransactionIndex: 0},
	}

	// We have 3 appearances, each might have multiple AssetTransfers. We'll create a small set:
	transfers := []AssetTransfer{
		// For block=123 tx=0, an incoming amount
		{
			BlockNumber:      123,
			TransactionIndex: 0,
			AssetAddr:        base.HexToAddress("0x0"), // native ETH
			Amount:           *base.NewWei(50),
			Index:            "trace_0",
			FromAddress:      base.HexToAddress("0x1001"),
			ToAddress:        base.HexToAddress("0x1002"),
		},
		// For block=123 tx=1, an outgoing amount
		{
			BlockNumber:      123,
			TransactionIndex: 1,
			AssetAddr:        base.HexToAddress("0x0"),
			Amount:           *base.NewWei(20),
			Index:            "trace_1",
			FromAddress:      base.HexToAddress("0x1002"),
			ToAddress:        base.HexToAddress("0x1003"),
		},
		// For block=124 tx=0, an incoming amount
		{
			BlockNumber:      124,
			TransactionIndex: 0,
			AssetAddr:        base.HexToAddress("0x0"),
			Amount:           *base.NewWei(100),
			Index:            "trace_2",
			FromAddress:      base.HexToAddress("0x1004"),
			ToAddress:        base.HexToAddress("0x1002"),
		},
	}

	// Process the appearances + transfers
	rec.ProcessAppearances(appearances, transfers)

	// We should now have a LedgerBook with one Ledger (asset "0x0").
	if len(rec.LedgerBook.Ledgers) != 1 {
		t.Fatalf("Expected 1 ledger, got=%d", len(rec.LedgerBook.Ledgers))
	}

	led, ok := rec.LedgerBook.GetLedger("0x0")
	if !ok {
		t.Fatalf("Expected a ledger for asset=0x0")
	}

	// We appended 3 appearances => 3 LedgerEntries
	if len(led.Entries) != 3 {
		t.Fatalf("Expected 3 ledger entries, got=%d", len(led.Entries))
	}

	totalIn := led.TotalIn()
	if totalIn.String() != "150" {
		t.Fatalf("Expected totalIn=150, got=%s", totalIn.String())
	}

	totalOut := led.TotalOut()
	if totalOut.String() != "20" {
		t.Fatalf("Expected totalOut=20, got=%s", totalOut.String())
	}

	netVal := led.NetValue()
	if netVal.String() != "130" {
		t.Fatalf("Expected netVal=130, got=%s", netVal.String())
	}
}

// TestReconcilerNoMatch tests if we pass an AssetTransfer that doesn't match
// one of our Appearances, we skip it.
func TestReconcilerNoMatch(t *testing.T) {
	rec := NewReconciler(base.HexToAddress("0x1002"))
	appearances := []types.Appearance{
		{Address: base.HexToAddress("0x1002"), BlockNumber: 10, TransactionIndex: 5},
	}

	// This transfer references block=10, tx=6 => does not match Appearances
	transfers := []AssetTransfer{
		{
			BlockNumber:      10,
			TransactionIndex: 6,
			AssetAddr:        base.HexToAddress("0x0"),
			Amount:           *base.NewWei(10),
			Index:            "trace_999",
			FromAddress:      base.HexToAddress("0x1006"),
			ToAddress:        base.HexToAddress("0x1002"),
		},
	}

	rec.ProcessAppearances(appearances, transfers)

	// We do have 1 appearance in appearances, but no matching transfer => no ledger entries
	if len(rec.LedgerBook.Ledgers) != 0 {
		t.Fatalf("Expected 0 ledgers, got=%d", len(rec.LedgerBook.Ledgers))
	}
}
