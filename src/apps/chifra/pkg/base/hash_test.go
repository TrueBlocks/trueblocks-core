package base

import (
	"fmt"
	"testing"

	"github.com/ethereum/go-ethereum/common"
)

func TestHash_Hex(t *testing.T) {
	hex := "0xF1AA581F353005BA3765B81BF52D6B1C488C2101"
	var hash Hash
	hash.SetHex(hex)

	expected := "0x000000000000000000000000f1aa581f353005ba3765b81bf52d6b1c488c2101"
	if result := hash.Hex(); result != expected {
		t.Fatal("wrong Hex() return value:", result)
	}

	zero := "0x000000000000"
	expected = "0x0000000000000000000000000000000000000000000000000000000000000000"
	hash = HexToHash(zero)
	if result := hash.Hex(); result != expected {
		t.Fatal("wrong Hex() return value for zero hash:", result)
	}

	var zero2 Hash
	hashStr := fmt.Sprint(zero2)
	if hashStr != expected {
		t.Fatal("wrong Hex() return value for zero hash:", hashStr)
	}
}

func TestHash_Stringer(t *testing.T) {
	hex := "0xF1AA581F353005BA3765B81BF52D6B1C488C2101"
	var hash Hash
	hash.SetHex(hex)

	expected := "0x000000000000000000000000f1aa581f353005ba3765b81bf52d6b1c488c2101"
	if result := fmt.Sprint(hash); result != expected {
		t.Fatal("wrong Hex() return value:", result)
	}
}

func TestHash_IsZero(t *testing.T) {
	var zeroValue Hash
	if result := zeroValue.IsZero(); result != true {
		t.Fatal("wrong result for zero value")
	}

	zeroHash := HexToHash("0x0")
	if result := zeroHash.IsZero(); result != true {
		t.Fatal("wrong result for zero hash")
	}
}

func TestHexToHash(t *testing.T) {
	hex := "0xF1AA581F353005BA3765B81BF52D6B1C488C2101"
	hash := HexToHash(hex)

	expected := "0x000000000000000000000000f1aa581f353005ba3765b81bf52d6b1c488c2101"
	if result := hash.Hex(); result != expected {
		t.Fatal("HexToHash: wrong Hex() return value:", result)
	}
}

func TestHashCompareToCommon(t *testing.T) {
	c := common.HexToHash("0x00000123456789abcde")
	b := HexToHash("0x00000123456789abcde")
	if c != b.ToCommon() {
		t.Fatal("base.Hash.toCommon() does not match")
	}
	if b != new(Hash).FromCommon(&c) {
		t.Fatal("fromCommon(c) does not match Hash")
	}
}
