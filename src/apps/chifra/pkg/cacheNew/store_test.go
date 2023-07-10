package cacheNew

import (
	"io"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/binary"
)

// We have to create a struct that implements Cache(un)Marshaler and Locator
// interfaces, i.e. is cacheable.
type testStoreData struct {
	Id    string
	Value string
}

func (t *testStoreData) MarshalCache(writer io.Writer) error {
	if err := binary.WriteValue(writer, t.Value); err != nil {
		return err
	}
	return nil
}

func (t *testStoreData) UnmarshalCache(version uint64, reader io.Reader) error {
	if err := binary.ReadValue(reader, &t.Value, version); err != nil {
		return err
	}
	return nil
}

func (t *testStoreData) CacheLocation() (string, string) {
	return "test", "bin"
}

func (t *testStoreData) CacheId() string {
	return t.Id
}

func (t *testStoreData) CacheName() string {
	return "StoreDataTest"
}

func TestStoreWrite(t *testing.T) {
	value := &testStoreData{
		Id:    "1",
		Value: "trueblocks",
	}
	opts := &StoreOptions{
		Location: MemoryCache,
	}
	cacheStore, err := NewStore(opts)
	if err != nil {
		panic(err)
	}

	// Write
	if err := cacheStore.Write(value, nil); err != nil {
		t.Fatal(err)
	}

	// Retrieve the same value
	result := new(testStoreData)
	if err := cacheStore.Read(result, "1", nil); err != nil {
		t.Fatal(err)
	}
	if result.Value != value.Value {
		t.Fatal("wrong value:", result.Value)
	}
}
