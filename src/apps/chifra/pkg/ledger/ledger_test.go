package ledger

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func TestGetOrCreateAssetBalancer_New(t *testing.T) {
	l := &Ledger{
		reversed:      false,
		appContexts:   make(map[appContextKey]*appContext),
		assetContexts: make(map[assetContextKey]*assetContext),
	}

	bn := base.Blknum(100)
	txid := base.Txnum(1)
	assetAddr := base.HexToAddress("0xabcdefabcdefabcdefabcdefabcdefabcdefabcd")

	assetCtx := l.getOrCreateAssetBalancer(bn, txid, assetAddr)

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

func TestGetOrCreateAssetBalancer_Existing(t *testing.T) {
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

	assetCtx1 := l.getOrCreateAssetBalancer(bn, txid, assetAddr)
	if assetCtx1 == nil {
		t.Fatal("Expected asset context to be non-nil")
	}

	assetCtx2 := l.getOrCreateAssetBalancer(bn, txid, assetAddr)
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

// func TestNewLedger_WithAssetFiltersNil(t *testing.T) {
// 	// Create a slice of dummy appearances.
// 	apps := []types.Appearance{
// 		{BlockNumber: 10, TransactionIndex: 1},
// 		{BlockNumber: 20, TransactionIndex: 2},
// 		{BlockNumber: 30, TransactionIndex: 3},
// 	}
// 	conn := rpc.TempConnection("mainnet")
// 	l := NewLedger(conn, apps, base.HexToAddress("0xAAA"), 10, 30, false, true, false, false, false, nil)
// 	// Expect that assetFilter is an empty slice.
// 	if len(l.assetFilter) != 0 {
// 		t.Errorf("Expected assetFilter to be empty when assetFilters is nil, got length %d", len(l.assetFilter))
// 	}
// 	// Verify appContexts for each appearance.
// 	if len(l.appContexts) != len(apps) {
// 		t.Errorf("Expected %d appContexts, got %d", len(apps), len(l.appContexts))
// 	}
// 	// For each appearance, check the computed prev, cur, and next block numbers.
// 	for index, app := range apps {
// 		expectedKey := fmt.Sprintf("%09d-%05d", app.BlockNumber, app.TransactionIndex)
// 		ctx, exists := l.appContexts[appContextKey(expectedKey)]
// 		if !exists {
// 			t.Errorf("Expected appContext with key %s to exist", expectedKey)
// 			continue
// 		}
// 		// For the first appearance, prev and cur should equal its block number,
// 		// and next should equal the block number of the second appearance.
// 		if index == 0 {
// 			if ctx.Prev() != base.Blknum(app.BlockNumber) {
// 				t.Errorf("For first app, expected Prev() to be %d, got %d", app.BlockNumber, ctx.Prev())
// 			}
// 			if ctx.Cur() != base.Blknum(app.BlockNumber) {
// 				t.Errorf("For first app, expected Cur() to be %d, got %d", app.BlockNumber, ctx.Cur())
// 			}
// 			if ctx.Next() != base.Blknum(apps[1].BlockNumber) {
// 				t.Errorf("For first app, expected Next() to be %d, got %d", apps[1].BlockNumber, ctx.Next())
// 			}
// 		}
// 		// For a middle appearance, prev should be first app's block number,
// 		// cur equal to its block, and next equal to the third app's block number.
// 		if index == 1 {
// 			if ctx.Prev() != base.Blknum(apps[0].BlockNumber) {
// 				t.Errorf("For middle app, expected Prev() to be %d, got %d", apps[0].BlockNumber, ctx.Prev())
// 			}
// 			if ctx.Cur() != base.Blknum(app.BlockNumber) {
// 				t.Errorf("For middle app, expected Cur() to be %d, got %d", app.BlockNumber, ctx.Cur())
// 			}
// 			if ctx.Next() != base.Blknum(apps[2].BlockNumber) {
// 				t.Errorf("For middle app, expected Next() to be %d, got %d", apps[2].BlockNumber, ctx.Next())
// 			}
// 		}
// 		// For the last appearance, prev should be the second app's block number,
// 		// cur equal to its block, and next equal to its block.
// 		if index == 2 {
// 			if ctx.Prev() != base.Blknum(apps[1].BlockNumber) {
// 				t.Errorf("For last app, expected Prev() to be %d, got %d", apps[1].BlockNumber, ctx.Prev())
// 			}
// 			if ctx.Cur() != base.Blknum(app.BlockNumber) {
// 				t.Errorf("For last app, expected Cur() to be %d, got %d", app.BlockNumber, ctx.Cur())
// 			}
// 			if ctx.Next() != base.Blknum(app.BlockNumber) {
// 				t.Errorf("For last app, expected Next() to be %d, got %d", app.BlockNumber, ctx.Next())
// 			}
// 		}
// 	}
// }

func TestNewLedger_WithAssetFiltersProvided(t *testing.T) {
	apps := []types.Appearance{
		{BlockNumber: 100, TransactionIndex: 1},
		{BlockNumber: 200, TransactionIndex: 2},
	}
	filters := []string{"0x1111111111111111111111111111111111111111", "0x2222222222222222222222222222222222222222"}
	conn := rpc.TempConnection("mainnet")
	l := NewLedger(conn, apps, base.HexToAddress("0xAAA"), 100, 200, false, true, false, false, false, &filters)
	// Check that assetFilter in Ledger is populated correctly.
	if len(l.assetFilter) != len(filters) {
		t.Errorf("Expected assetFilter length %d, got %d", len(filters), len(l.assetFilter))
	}
	for i, addrStr := range filters {
		expectedAddr := base.HexToAddress(addrStr)
		if l.assetFilter[i] != expectedAddr {
			t.Errorf("At index %d, expected assetFilter %s, got %s", i, expectedAddr.Hex(), l.assetFilter[i].Hex())
		}
	}
	// Also verify that appContexts are created.
	if len(l.appContexts) != len(apps) {
		t.Errorf("Expected %d appContexts, got %d", len(apps), len(l.appContexts))
	}
}
