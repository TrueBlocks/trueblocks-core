package ledger1

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger4"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
)

func TestNewLedger_WithAssetFiltersProvided(t *testing.T) {
	filters := []base.Address{
		base.HexToAddress("0x1111111111111111111111111111111111111111"),
		base.HexToAddress("0x2222222222222222222222222222222222222222")}
	conn := rpc.TempConnection("mainnet")
	l := NewReconciler1(&ledger4.ReconcilerOptions{
		Connection:   conn,
		AccountFor:   base.HexToAddress("0xAAA"),
		FirstBlock:   100,
		LastBlock:    200,
		AsEther:      false,
		TestMode:     true,
		UseTraces:    false,
		Reversed:     false,
		AssetFilters: filters,
	})
	// Check that assetFilter in Reconciler1 is populated correctly.
	if len(l.assetFilter) != len(filters) {
		t.Errorf("Expected assetFilter length %d, got %d", len(filters), len(l.assetFilter))
	}
	for i, expectedAddr := range filters {
		if l.assetFilter[i] != expectedAddr {
			t.Errorf("At index %d, expected assetFilter %s, got %s", i, expectedAddr.Hex(), l.assetFilter[i].Hex())
		}
	}
}
