package types

import (
	"testing"

	"github.com/ethereum/go-ethereum/common"
)

func TestTypes(t *testing.T) {
	r := SimpleReceipt{
		BlockHash:   common.Hash{0x1},
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
