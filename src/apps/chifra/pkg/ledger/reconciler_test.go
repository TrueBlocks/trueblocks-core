package ledger

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
)

func TestNewLedger_WithAssetFiltersProvided(t *testing.T) {
	filters := []string{"0x1111111111111111111111111111111111111111", "0x2222222222222222222222222222222222222222"}
	conn := rpc.TempConnection("mainnet")
	l := NewReconciler(conn, base.HexToAddress("0xAAA"), 100, 200, false, true, false, false, &filters)
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
