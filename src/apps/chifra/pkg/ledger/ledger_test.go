package ledger

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func TestAssetOfInterest(t *testing.T) {
	// Case 1: Empty assetFilter should always return true.
	l1 := &Ledger{
		assetFilter: []base.Address{},
	}
	needle := base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678")
	if !l1.assetOfInterest(needle) {
		t.Error("Expected assetOfInterest to return true when assetFilter is empty")
	}

	// Case 2: Non-empty assetFilter where needle is present.
	l2 := &Ledger{
		assetFilter: []base.Address{
			base.HexToAddress("0xabcdefabcdefabcdefabcdefabcdefabcdefabcd"),
			base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678"), // needle below
			base.HexToAddress("0xdeadbeefdeadbeefdeadbeefdeadbeefdeadbeef"),
		},
	}
	needle2 := base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678")
	if !l2.assetOfInterest(needle2) {
		t.Error("Expected assetOfInterest to return true when needle is present in assetFilter")
	}

	// Case 3: Non-empty assetFilter where needle is not present.
	l3 := &Ledger{
		assetFilter: []base.Address{
			base.HexToAddress("0xabcdefabcdefabcdefabcdefabcdefabcdefabcd"),
			base.HexToAddress("0xdeadbeefdeadbeefdeadbeefdeadbeefdeadbeef"),
		},
	}
	needle3 := base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678")
	if l3.assetOfInterest(needle3) {
		t.Error("Expected assetOfInterest to return false when needle is not present in assetFilter")
	}

	// Case 4: Multiple entries with mixed ordering. (This is similar to Case 2.)
	l4 := &Ledger{
		assetFilter: []base.Address{
			base.HexToAddress("0xdeadbeefdeadbeefdeadbeefdeadbeefdeadbeef"),
			base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678"),
			base.HexToAddress("0xabcdefabcdefabcdefabcdefabcdefabcdefabcd"),
		},
	}
	if !l4.assetOfInterest(needle2) {
		t.Error("Expected assetOfInterest to return true when needle is present among multiple addresses")
	}
}
