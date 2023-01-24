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

func TestAddress_Hex(t *testing.T) {
	hex := "0xF1AA581F353005BA3765B81BF52D6B1C488C2101"
	var addr Address
	addr.SetHex(hex)

	expected := "0xf1aa581f353005ba3765b81bf52d6b1c488c2101"
	if result := addr.Hex(); result != expected {
		t.Fatal("wrong Hex() return value:", result)
	}
}

func TestAddress_IsZero(t *testing.T) {
	var zeroValue Address
	if result := zeroValue.IsZero(); result != true {
		t.Fatal("wrong result for zero value")
	}

	zeroAddr := HexToAddress("0x0")
	if result := zeroAddr.IsZero(); result != true {
		t.Fatal("wrong result for zero address")
	}
}

func TestHexToAddress(t *testing.T) {
	hex := "0xF1AA581F353005BA3765B81BF52D6B1C488C2101"
	addr := HexToAddress(hex)

	expected := "0xf1aa581f353005ba3765b81bf52d6b1c488c2101"
	if result := addr.Hex(); result != expected {
		t.Fatal("HexToAddress: wrong Hex() return value:", result)
	}
}
