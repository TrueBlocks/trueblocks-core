package cache

import (
	"bytes"
	"io"
	"reflect"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// We have to create a struct that implements Cache(un)Marshaler and Locator
// interfaces, i.e. is cacheable.
type testStoreData struct {
	Id    string
	Value string
}

func (t *testStoreData) MarshalCache(writer io.Writer) error {
	if err := base.WriteValue(writer, t.Value); err != nil {
		return err
	}
	return nil
}

func (t *testStoreData) UnmarshalCache(fileVersion uint64, reader io.Reader) error {
	if err := base.ReadValue(reader, &t.Value, fileVersion); err != nil {
		return err
	}
	return nil
}

func (t *testStoreData) CacheLocations() (string, string, string) {
	return "test", t.Id, "bin"
}

func TestStoreWrite(t *testing.T) {
	value := &testStoreData{
		Id:    "1",
		Value: "trueblocks",
	}
	cacheStore, err := NewStore(&StoreOptions{Location: MemoryCache})
	if err != nil {
		panic(err)
	}

	// Write
	if err := cacheStore.Write(value); err != nil {
		t.Fatal(err)
	}

	// Retrieve the same value
	result := &testStoreData{
		Id: "1",
	}
	if err := cacheStore.Read(result); err != nil {
		t.Fatal(err)
	}
	if result.Value != value.Value {
		t.Fatal("wrong value:", result.Value)
	}
}

func TestAddressCache(t *testing.T) {
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
