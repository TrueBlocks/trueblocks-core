package base

import (
	"bytes"
	"fmt"
	"testing"

	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

func TestAddress_Hex(t *testing.T) {
	a := hexutil.MustDecode("0xAb")
	b := []byte{0xAb}
	c := bytes.Compare(a, b)
	if c != 0 {
		t.Error("incorrect result from MustDecode")
	}

	addr := HexToAddress("0x00000000000000000000000000000000deadbeef")

	hex := "0xF1AA581F353005BA3765B81BF52D6B1C488C2101"
	addr.SetHex(hex)

	expectedHex := "0xf1aa581f353005ba3765b81bf52d6b1c488c2101"
	if result := addr.Hex(); result != expectedHex {
		t.Fatal("wrong Hex() return value:", result)
	}

	zero := "0x0000000000000000000000000000000000000000"
	expectedHex = "0x0"
	addr = HexToAddress(zero)
	if result := addr.Hex(); result != expectedHex {
		t.Fatal("wrong Hex() return value for 0x0:", result)
	}

	var zero2 Address
	addrStr := fmt.Sprint(zero2)
	if addrStr != expectedHex {
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

	if result := ZeroAddr.IsZero(); result != true {
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
	if c != b.Common() {
		t.Fatal("base.Hash.toCommon() does not match")
	}
	if b != new(Address).SetCommon(&c) {
		t.Fatal("fromCommon(c) does not match Hash")
	}
}
