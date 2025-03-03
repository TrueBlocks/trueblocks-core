package ledger3

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func TestAssetOfInterest(t *testing.T) {
	// Case 1: Empty assetFilter should always return true.
	af1 := []base.Address{}
	needle := base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678")
	if !AssetOfInterest(af1, needle) {
		t.Error("Expected assetOfInterest to return true when assetFilter is empty")
	}

	// Case 2: Non-empty assetFilter where needle is present.
	af2 := []base.Address{
		base.HexToAddress("0xabcdefabcdefabcdefabcdefabcdefabcdefabcd"),
		base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678"), // needle below
		base.HexToAddress("0xdeadbeefdeadbeefdeadbeefdeadbeefdeadbeef"),
	}
	needle2 := base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678")
	if !AssetOfInterest(af2, needle2) {
		t.Error("Expected assetOfInterest to return true when needle is present in assetFilter")
	}

	// Case 3: Non-empty assetFilter where needle is not present.
	af3 := []base.Address{
		base.HexToAddress("0xabcdefabcdefabcdefabcdefabcdefabcdefabcd"),
		base.HexToAddress("0xdeadbeefdeadbeefdeadbeefdeadbeefdeadbeef"),
	}
	needle3 := base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678")
	if AssetOfInterest(af3, needle3) {
		t.Error("Expected assetOfInterest to return false when needle is not present in assetFilter")
	}

	// Case 4: Multiple entries with mixed ordering. (This is similar to Case 2.)
	af4 := []base.Address{
		base.HexToAddress("0xdeadbeefdeadbeefdeadbeefdeadbeefdeadbeef"),
		base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678"),
		base.HexToAddress("0xabcdefabcdefabcdefabcdefabcdefabcdefabcd"),
	}
	if !AssetOfInterest(af4, needle2) {
		t.Error("Expected assetOfInterest to return true when needle is present among multiple addresses")
	}
}
