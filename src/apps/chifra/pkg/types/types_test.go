package types

import (
	"testing"

	base "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func TestTypes(t *testing.T) {
	s := SimpleReceipt{
		BlockHash:   base.HexToHash("0x1"),
		BlockNumber: 100,
	}

	data := s.Model(false, "csv", nil).Data

	if data["blockHash"] != nil {
		t.Fatal("hidden field leaked")
	}

	if data["blockNumber"] == nil {
		t.Fatal("visible field missing")
	}

	data = s.Model(true, "json", nil).Data
	if data["blockHash"] == nil {
		t.Fatal("hidden field missing when verbose = true")
	}

	if data["blockNumber"] == nil {
		t.Fatal("visible field missing when verbose = true")
	}
}
