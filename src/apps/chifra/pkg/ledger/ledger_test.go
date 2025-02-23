package ledger

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger2"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
)

func TestAssetOfInterest(t *testing.T) {
	// Case 1: Empty assetFilter should always return true.
	l1 := &Reconciler{
		assetFilter: []base.Address{},
	}
	needle := base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678")
	if !ledger2.AssetOfInterest(l1.assetFilter, needle) {
		t.Error("Expected assetOfInterest to return true when assetFilter is empty")
	}

	// Case 2: Non-empty assetFilter where needle is present.
	l2 := &Reconciler{
		assetFilter: []base.Address{
			base.HexToAddress("0xabcdefabcdefabcdefabcdefabcdefabcdefabcd"),
			base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678"), // needle below
			base.HexToAddress("0xdeadbeefdeadbeefdeadbeefdeadbeefdeadbeef"),
		},
	}
	needle2 := base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678")
	if !ledger2.AssetOfInterest(l2.assetFilter, needle2) {
		t.Error("Expected assetOfInterest to return true when needle is present in assetFilter")
	}

	// Case 3: Non-empty assetFilter where needle is not present.
	l3 := &Reconciler{
		assetFilter: []base.Address{
			base.HexToAddress("0xabcdefabcdefabcdefabcdefabcdefabcdefabcd"),
			base.HexToAddress("0xdeadbeefdeadbeefdeadbeefdeadbeefdeadbeef"),
		},
	}
	needle3 := base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678")
	if ledger2.AssetOfInterest(l3.assetFilter, needle3) {
		t.Error("Expected assetOfInterest to return false when needle is not present in assetFilter")
	}

	// Case 4: Multiple entries with mixed ordering. (This is similar to Case 2.)
	l4 := &Reconciler{
		assetFilter: []base.Address{
			base.HexToAddress("0xdeadbeefdeadbeefdeadbeefdeadbeefdeadbeef"),
			base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678"),
			base.HexToAddress("0xabcdefabcdefabcdefabcdefabcdefabcdefabcd"),
		},
	}
	if !ledger2.AssetOfInterest(l4.assetFilter, needle2) {
		t.Error("Expected assetOfInterest to return true when needle is present among multiple addresses")
	}
}

func TestNewLedger_WithAssetFiltersProvided(t *testing.T) {
	filters := []string{"0x1111111111111111111111111111111111111111", "0x2222222222222222222222222222222222222222"}
	conn := rpc.TempConnection("mainnet")
	l := NewLedger(conn, base.HexToAddress("0xAAA"), 100, 200, false, true, false, false, false, &filters)
	// Check that assetFilter in Reconciler is populated correctly.
	if len(l.assetFilter) != len(filters) {
		t.Errorf("Expected assetFilter length %d, got %d", len(filters), len(l.assetFilter))
	}
	for i, addrStr := range filters {
		expectedAddr := base.HexToAddress(addrStr)
		if l.assetFilter[i] != expectedAddr {
			t.Errorf("At index %d, expected assetFilter %s, got %s", i, expectedAddr.Hex(), l.assetFilter[i].Hex())
		}
	}
}
