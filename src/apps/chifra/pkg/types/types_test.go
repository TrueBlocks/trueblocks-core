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

	if r.Model(false, "csv").Data["blockHash"] != nil {
		t.Fatal("hidden field leaked")
	}

	if r.Model(false, "csv").Data["blockNumber"] == nil {
		t.Fatal("visible field missing")
	}

	if r.Model(true, "json").Data["blockHash"] == nil {
		t.Fatal("hidden field missing when showHidden = true")
	}

	if r.Model(true, "csv").Data["blockNumber"] == nil {
		t.Fatal("visible field missing when showHidden = true")
	}
}
