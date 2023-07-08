package base

import (
	"fmt"
	"testing"

	"github.com/ethereum/go-ethereum/common"
)

func TestAddress_Hex(t *testing.T) {
	hex := "0xF1AA581F353005BA3765B81BF52D6B1C488C2101"
	var addr Address
	addr.SetHex(hex)

	expected := "0xf1aa581f353005ba3765b81bf52d6b1c488c2101"
	if result := addr.Hex(); result != expected {
		t.Fatal("wrong Hex() return value:", result)
	}

	zero := "0x0000000000000000000000000000000000000000"
	expected = "0x0"
	addr = HexToAddress(zero)
	if result := addr.Hex(); result != expected {
		t.Fatal("wrong Hex() return value for 0x0:", result)
	}

	var zero2 Address
	addrStr := fmt.Sprint(zero2)
	if addrStr != expected {
		t.Fatal("wrong Hex() return value for 0x0:", addrStr)
	}
}

func TestAddress_Stringer(t *testing.T) {
	hex := "0xF1AA581F353005BA3765B81BF52D6B1C488C2101"
	var addr Address
	addr.SetHex(hex)

	expected := "0xf1aa581f353005ba3765b81bf52d6b1c488c2101"
	if result := fmt.Sprint(addr); result != expected {
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

func TestAddressCompareToCommon(t *testing.T) {
	c := common.HexToAddress("0x00000123456789abcde")
	b := HexToAddress("0x00000123456789abcde")
	if c != b.ToCommon() {
		t.Fatal("base.Hash.toCommon() does not match")
	}
	if b != new(Address).FromCommon(&c) {
		t.Fatal("fromCommon(c) does not match Hash")
	}
}
