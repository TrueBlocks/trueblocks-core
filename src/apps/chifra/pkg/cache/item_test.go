package cache

import (
	"bytes"
	"reflect"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// Address works out of the box because internally it's just a slice of bytes,
// i.e. something that binary.Write can encode
func TestItemAddress(t *testing.T) {
	buf := new(bytes.Buffer)
	item := NewItem(buf)

	addr := base.HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b")
	if err := item.Encode(addr); err != nil {
		t.Fatal(err)
	}
	result := buf.Bytes()[HeaderByteSize:]
	if !reflect.DeepEqual(result, addr.Bytes()) {
		t.Fatalf("values are not same: got %x, expected %x", result, addr.Bytes())
	}
	readerItem := NewItem(buf)
	var readAddr base.Address
	if err := readerItem.Decode(&readAddr); err != nil {
		return
	}
	if readAddr.Hex() != addr.Hex() {
		t.Fatal("read wrong value:", readAddr)
	}

	// 0x0 case
	zeroAddr := base.Address{}
	buf.Reset()
	if err := item.Encode(zeroAddr); err != nil {
		t.Fatal(err)
	}
	resultAddr := base.BytesToAddress(buf.Bytes()[HeaderByteSize:])
	if !resultAddr.IsZero() {
		t.Fatalf("expected zero address, but got %s", resultAddr)
	}
}
