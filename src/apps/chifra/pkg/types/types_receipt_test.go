package types

import (
	"testing"

	base "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/utils"
)

func TestTypes(t *testing.T) {
	chain := utils.GetTestChain()
	s := Receipt{
		BlockHash:   base.HexToHash("0x1"),
		BlockNumber: 100,
	}

	data := s.Model(chain, "csv", false, nil).Data

	if data["blockHash"] != nil {
		t.Fatal("hidden field leaked")
	}

	if data["blockNumber"] == nil {
		t.Fatal("visible field missing")
	}

	data = s.Model(chain, "json", true, nil).Data
	if data["blockHash"] == nil {
		t.Fatal("hidden field missing when verbose = true")
	}

	if data["blockNumber"] == nil {
		t.Fatal("visible field missing when verbose = true")
	}
}
