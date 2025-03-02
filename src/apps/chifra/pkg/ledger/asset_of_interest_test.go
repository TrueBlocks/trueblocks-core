package ledger

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger3"
)

func TestAssetOfInterest(t *testing.T) {
	// Case 1: Empty assetFilter should always return true.
	r1 := &Reconciler{
		assetFilter: []base.Address{},
	}
	needle := base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678")
	if !ledger3.AssetOfInterest(r1.assetFilter, needle) {
		t.Error("Expected assetOfInterest to return true when assetFilter is empty")
	}

	// Case 2: Non-empty assetFilter where needle is present.
	r2 := &Reconciler{
		assetFilter: []base.Address{
			base.HexToAddress("0xabcdefabcdefabcdefabcdefabcdefabcdefabcd"),
			base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678"), // needle below
			base.HexToAddress("0xdeadbeefdeadbeefdeadbeefdeadbeefdeadbeef"),
		},
	}
	needle2 := base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678")
	if !ledger3.AssetOfInterest(r2.assetFilter, needle2) {
		t.Error("Expected assetOfInterest to return true when needle is present in assetFilter")
	}

	// Case 3: Non-empty assetFilter where needle is not present.
	r3 := &Reconciler{
		assetFilter: []base.Address{
			base.HexToAddress("0xabcdefabcdefabcdefabcdefabcdefabcdefabcd"),
			base.HexToAddress("0xdeadbeefdeadbeefdeadbeefdeadbeefdeadbeef"),
		},
	}
	needle3 := base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678")
	if ledger3.AssetOfInterest(r3.assetFilter, needle3) {
		t.Error("Expected assetOfInterest to return false when needle is not present in assetFilter")
	}

	// Case 4: Multiple entries with mixed ordering. (This is similar to Case 2.)
	r4 := &Reconciler{
		assetFilter: []base.Address{
			base.HexToAddress("0xdeadbeefdeadbeefdeadbeefdeadbeefdeadbeef"),
			base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678"),
			base.HexToAddress("0xabcdefabcdefabcdefabcdefabcdefabcdefabcd"),
		},
	}
	if !ledger3.AssetOfInterest(r4.assetFilter, needle2) {
		t.Error("Expected assetOfInterest to return true when needle is present among multiple addresses")
	}
}

