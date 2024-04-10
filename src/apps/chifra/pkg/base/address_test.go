package base

import (
	"bytes"
	"fmt"
	"reflect"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
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

func TestAddressCache(t *testing.T) {
	buf := new(bytes.Buffer)
	item := cache.NewItem(buf)

	addr := HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b")
	if err := item.Encode(addr); err != nil {
		t.Fatal(err)
	}
	result := buf.Bytes()[cache.HeaderByteSize:]
	if !reflect.DeepEqual(result, addr.Bytes()) {
		t.Fatalf("values are not same: got %x, expected %x", result, addr.Bytes())
	}
	readerItem := cache.NewItem(buf)
	var readAddr Address
	if err := readerItem.Decode(&readAddr); err != nil {
		return
	}
	if readAddr.Hex() != addr.Hex() {
		t.Fatal("read wrong value:", readAddr)
	}

	// 0x0 case
	zeroAddr := Address{}
	buf.Reset()
	if err := item.Encode(zeroAddr); err != nil {
		t.Fatal(err)
	}
	resultAddr := BytesToAddress(buf.Bytes()[cache.HeaderByteSize:])
	if !resultAddr.IsZero() {
		t.Fatalf("expected zero address, but got %s", resultAddr)
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
