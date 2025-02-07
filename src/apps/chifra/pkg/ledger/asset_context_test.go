package ledger

import (
	"fmt"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func TestGetAssetContextKey(t *testing.T) {
	addr := base.HexToAddress("0xabcdefabcdefabcdefabcdefabcdefabcdefabcd")
	bn := base.Blknum(123)
	txid := base.Txnum(45)
	expected := fmt.Sprintf("%s-%09d-%05d", addr.Hex(), bn, txid)
	var l Ledger
	key := l.getAssetContextKey(bn, txid, addr)
	if string(key) != expected {
		t.Errorf("Expected %s, got %s", expected, key)
	}
}
