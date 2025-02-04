package ledger

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func TestGetOrCreateAssetContext_New(t *testing.T) {
	l := &Ledger{
		reversed:      false,
		appContexts:   make(map[appContextKey]*appContext),
		assetContexts: make(map[assetContextKey]*assetContext),
	}

	bn := base.Blknum(100)
	txid := base.Txnum(1)
	assetAddr := base.HexToAddress("0xabcdefabcdefabcdefabcdefabcdefabcdefabcd")

	assetCtx := l.getOrCreateAssetContext(bn, txid, assetAddr)

	if assetCtx == nil {
		t.Fatal("Expected asset context to be non-nil")
	}

	if assetCtx.address != assetAddr {
		t.Errorf("Expected asset address %s, got %s", assetAddr.Hex(), assetCtx.address.Hex())
	}

	appKey := l.getAppContextKey(bn, txid)
	if _, exists := l.appContexts[appKey]; !exists {
		t.Error("Expected app context to be created and stored in ledger.appContexts")
	}

	assetKey := l.getAssetContextKey(bn, txid, assetAddr)
	storedAssetCtx, exists := l.assetContexts[assetKey]
	if !exists {
		t.Error("Expected asset context to be stored in ledger.assetContexts")
	}
	if storedAssetCtx != assetCtx {
		t.Error("Stored asset context does not match returned asset context")
	}
}

func TestGetOrCreateAssetContext_Existing(t *testing.T) {
	l := &Ledger{
		reversed:      false,
		appContexts:   make(map[appContextKey]*appContext),
		assetContexts: make(map[assetContextKey]*assetContext),
	}

	bn := base.Blknum(200)
	txid := base.Txnum(2)
	assetAddr := base.HexToAddress("0x1111111111111111111111111111111111111111")

	appKey := l.getAppContextKey(bn, txid)
	fakeAppCtx := newAppContext(bn-1, bn, bn+1, false, false, l.reversed)
	l.appContexts[appKey] = fakeAppCtx

	assetCtx1 := l.getOrCreateAssetContext(bn, txid, assetAddr)
	if assetCtx1 == nil {
		t.Fatal("Expected asset context to be non-nil")
	}

	assetCtx2 := l.getOrCreateAssetContext(bn, txid, assetAddr)
	if assetCtx1 != assetCtx2 {
		t.Error("Expected subsequent calls to return the same asset context instance")
	}

	if assetCtx1.Prev() != fakeAppCtx.Prev() ||
		assetCtx1.Cur() != fakeAppCtx.Cur() ||
		assetCtx1.Next() != fakeAppCtx.Next() {
		t.Error("Asset context values do not match those of the underlying app context")
	}
}

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
