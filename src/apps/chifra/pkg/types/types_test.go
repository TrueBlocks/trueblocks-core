package types

import (
	"testing"

	base "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func TestTypes(t *testing.T) {
	r := SimpleReceipt{
		BlockHash:   base.HexToHash("0x1"),
		BlockNumber: 100,
	}

	data := r.Model(false, "csv", nil).Data

	if data["blockHash"] != nil {
		t.Fatal("hidden field leaked")
	}

	if data["blockNumber"] == nil {
		t.Fatal("visible field missing")
	}

	data = r.Model(true, "json", nil).Data
	if data["blockHash"] == nil {
		t.Fatal("hidden field missing when showHidden = true")
	}

	if data["blockNumber"] == nil {
		t.Fatal("visible field missing when showHidden = true")
	}
}
