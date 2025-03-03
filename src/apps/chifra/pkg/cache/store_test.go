package cache

import (
	"bytes"
	"fmt"
	"io"
	"reflect"
	"strings"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache/locations"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func TestStoreWrite(t *testing.T) {
	t.Run("HappyPath", func(t *testing.T) {
		store, err := newTestStore(true, walk.Cache_Results, true, 1000)
		if err != nil {
			t.Errorf("Failed to create store: %v", err)
		}
		data := &testStoreData{
			Id:    "1",
			Value: "direct",
		}
		err = store.Write(data)
		if err != nil {
			t.Errorf("Write failed: %v", err)
		}
		result := &testStoreData{
			Id: "1",
		}
		if err := store.Read(result); err != nil {
			t.Errorf("Read failed: %v", err)
		}
		if result.Value != "direct" {
			t.Errorf("Wrong value: %v", result.Value)
		}
	})
	t.Run("StoreDisabled", func(t *testing.T) {
		store, err := newTestStore(false, walk.Cache_Results, true, 1000)
		if err != nil {
			t.Errorf("Failed to create store: %v", err)
		}
		data := &testStoreData{
			Id:    "2",
			Value: "direct",
		}
		err = store.Write(data)
		if err != ErrReadOnly {
			t.Errorf("Expected ErrReadOnly, got %v", err)
		}
		result := &testStoreData{
			Id: "2",
		}
		if err := store.Read(result); err == nil {
			t.Errorf("Expected read to fail (no write occurred)")
		}
	})
	t.Run("InvalidPath", func(t *testing.T) {
		store, err := newTestStore(true, walk.Cache_Results, true, 1000)
		if err != nil {
			t.Errorf("Failed to create store: %v", err)
		}
		data := &testStoreData{
			Id:    "",
			Value: "direct",
		}
		err = store.Write(data)
		if err == nil {
			t.Errorf("Expected error, got nil")
		}
	})
}

func TestStoreRead(t *testing.T) {
	t.Run("HappyPath", func(t *testing.T) {
		store, err := newTestStore(true, walk.Cache_Results, true, 1000)
		if err != nil {
			t.Errorf("Failed to create store: %v", err)
		}
		data := &testStoreData{
			Id:    "1",
			Value: "readtest",
		}
		err = store.Write(data)
		if err != nil {
			t.Errorf("Write failed: %v", err)
		}
		result := &testStoreData{
			Id: "1",
		}
		err = store.Read(result)
		if err != nil {
			t.Errorf("Read failed: %v", err)
		}
		if result.Value != "readtest" {
			t.Errorf("Wrong value: %v", result.Value)
		}
	})
	t.Run("ReadNonExistent", func(t *testing.T) {
		store, err := newTestStore(true, walk.Cache_Results, true, 1000)
		if err != nil {
			t.Errorf("Failed to create store: %v", err)
		}
		result := &testStoreData{
			Id: "999",
		}
		err = store.Read(result)
		if err == nil {
			t.Errorf("Expected error, got nil")
		}
	})
}

func TestStoreStat(t *testing.T) {
	t.Run("HappyPath", func(t *testing.T) {
		store, err := newTestStore(true, walk.Cache_Results, true, 1000)
		if err != nil {
			t.Errorf("Failed to create store: %v", err)
		}
		data := &testStoreData{
			Id:    "1",
			Value: "stattest",
		}
		err = store.Write(data)
		if err != nil {
			t.Errorf("Write failed: %v", err)
		}
		info, err := store.Stat(data)
		if err != nil {
			t.Errorf("Stat failed: %v", err)
		}
		if info == nil {
			t.Errorf("Expected non-nil ItemInfo, got nil")
		}
	})

	t.Run("StatNonExistent", func(t *testing.T) {
		store, err := newTestStore(true, walk.Cache_Results, true, 1000)
		if err != nil {
			t.Errorf("Failed to create store: %v", err)
		}
		data := &testStoreData{
			Id: "999",
		}
		info, err := store.Stat(data)
		if err != nil {
			t.Errorf("Unexpected error")
		}
		if info.Size() != 0 {
			t.Errorf("Expected zero sized file %v", info)
		}
	})
}

func TestStoreRemove(t *testing.T) {
	t.Run("HappyPath", func(t *testing.T) {
		store, err := newTestStore(true, walk.Cache_Results, true, 1000)
		if err != nil {
			t.Errorf("Failed to create store: %v", err)
		}
		data := &testStoreData{
			Id:    "1",
			Value: "removetest",
		}
		err = store.Write(data)
		if err != nil {
			t.Errorf("Write failed: %v", err)
		}
		err = store.Remove(data)
		if err != nil {
			t.Errorf("Remove failed: %v", err)
		}
		result := &testStoreData{
			Id: "1",
		}
		if err := store.Read(result); err == nil {
			t.Errorf("Expected read to fail after remove, got no error")
		}
	})
	t.Run("RemoveNonExistent", func(t *testing.T) {
		store, err := newTestStore(true, walk.Cache_Results, true, 1000)
		if err != nil {
			t.Errorf("Failed to create store: %v", err)
		}
		data := &testStoreData{
			Id: "999",
		}
		err = store.Remove(data)
		if err != nil {
			t.Errorf("Expected no error, got %v", err)
		}
	})
}

func TestStoreDecache(t *testing.T) {
	t.Run("HappyPath", func(t *testing.T) {
		store, err := newTestStore(true, walk.Cache_Results, true, 1000)
		if err != nil {
			t.Errorf("Failed to create store: %v", err)
		}
		data1 := &testStoreData{
			Id:    "1",
			Value: "decache1",
		}
		data2 := &testStoreData{
			Id:    "2",
			Value: "decache2",
		}
		err = store.Write(data1)
		if err != nil {
			t.Errorf("Write failed: %v", err)
		}
		err = store.Write(data2)
		if err != nil {
			t.Errorf("Write failed: %v", err)
		}
		procFunc := func(*locations.ItemInfo) bool { return true }
		skipFunc := func(*locations.ItemInfo) bool { return false }
		locators := []Locator{data1, data2}
		err = store.Decache(locators, procFunc, skipFunc)
		if err != nil {
			t.Errorf("Decache failed: %v", err)
		}
		result1 := &testStoreData{Id: "1"}
		if err := store.Read(result1); err == nil {
			t.Errorf("Expected read to fail for data1 after decache")
		}
		result2 := &testStoreData{Id: "2"}
		if err := store.Read(result2); err == nil {
			t.Errorf("Expected read to fail for data2 after decache")
		}
	})
	t.Run("NonExistent", func(t *testing.T) {
		store, err := newTestStore(true, walk.Cache_Results, true, 1000)
		if err != nil {
			t.Errorf("Failed to create store: %v", err)
		}
		data := &testStoreData{
			Id: "999",
		}
		procFunc := func(*locations.ItemInfo) bool { return true }
		skipFunc := func(*locations.ItemInfo) bool { return false }
		locators := []Locator{data}
		err = store.Decache(locators, procFunc, skipFunc)
		if err != nil {
			t.Errorf("Expected no error, got %v", err)
		}
	})
	t.Run("ProcFuncSkips", func(t *testing.T) {
		store, err := newTestStore(true, walk.Cache_Results, true, 1000)
		if err != nil {
			t.Errorf("Failed to create store: %v", err)
		}
		data := &testStoreData{
			Id:    "3",
			Value: "decache3",
		}
		err = store.Write(data)
		if err != nil {
			t.Errorf("Write failed: %v", err)
		}
		procFunc := func(*locations.ItemInfo) bool { return false }
		skipFunc := func(*locations.ItemInfo) bool { return false }
		locators := []Locator{data}
		err = store.Decache(locators, procFunc, skipFunc)
		if err != nil {
			t.Errorf("Decache failed: %v", err)
		}
		result := &testStoreData{Id: "3"}
		if err := store.Read(result); err != nil {
			t.Errorf("Expected read to succeed (data not removed), got %v", err)
		}
		if result.Value != "decache3" {
			t.Errorf("Wrong value: %v", result.Value)
		}
	})
}

func TestStoreTestLog(t *testing.T) {
	t.Run("EnabledStore", func(t *testing.T) {
		store, err := newTestStore(true, walk.Cache_Results, true, 1000)
		if err != nil {
			t.Errorf("Failed to create store: %v", err)
		}
		store.TestLog()
	})
	t.Run("DisabledStore", func(t *testing.T) {
		store, err := newTestStore(false, walk.Cache_Results, true, 1000)
		if err != nil {
			t.Errorf("Failed to create store: %v", err)
		}
		store.TestLog()
	})
}

func TestStoreLatest(t *testing.T) {
	t.Run("SetLatestUpdates", func(t *testing.T) {
		store, err := newTestStore(true, walk.Cache_Results, true, 5000)
		if err != nil {
			t.Errorf("Failed to create store: %v", err)
		}
		if !store.IsFinal(900) {
			t.Errorf("Expected 900 to be final with latest 1000")
		}
		store.SetLatest(800)
		if store.IsFinal(900) {
			t.Errorf("Expected 900 to be non-final with latest 800")
		}
	})
	t.Run("ZeroLatest", func(t *testing.T) {
		store, err := newTestStore(true, walk.Cache_Results, true, 0)
		if err != nil {
			t.Errorf("Failed to create store: %v", err)
		}
		if store.IsFinal(1000) {
			t.Errorf("Expected 1000 to not be final with latest 0")
		}
	})
}

func TestStorePathResolution(t *testing.T) {
	t.Run("RelativePath", func(t *testing.T) {
		store, err := newTestStore(true, walk.Cache_Results, true, 1000)
		if err != nil {
			t.Errorf("Failed to create store: %v", err)
		}
		data := &testStoreData{
			Id:    "1",
			Value: "relative",
		}
		err = store.Write(data)
		if err != nil {
			t.Errorf("Write failed: %v", err)
		}
		result := &testStoreData{
			Id: "1",
		}
		if err := store.Read(result); err != nil {
			t.Errorf("Read failed: %v", err)
		}
		if result.Value != "relative" {
			t.Errorf("Wrong value: %v", result.Value)
		}
	})
}

func TestStoreInit(t *testing.T) {
	t.Run("NewStoreSuccess", func(t *testing.T) {
		store, err := NewStore(&StoreOptions{
			Location:   MemoryCache,
			Enabled:    true,
			EnabledMap: map[walk.CacheType]bool{walk.Cache_Results: true},
			Latest:     1000,
		})
		if err != nil {
			t.Errorf("NewStore failed: %v", err)
		}
		if store == nil {
			t.Errorf("Expected non-nil store, got nil")
		}
		if !store.Enabled() {
			t.Errorf("Expected store to be enabled")
		}
	})
}

func TestWriteToCache(t *testing.T) {
	t.Run("HappyPath", func(t *testing.T) {
		store, err := newTestStore(true, walk.Cache_Results, true, 5000)
		if err != nil {
			t.Errorf("Failed to create store %v", err)
		}

		data := &testStoreData{
			Id:    "1",
			Value: "trueblocks",
		}
		ts := base.Timestamp(900)

		err = store.WriteToCache(data, walk.Cache_Results, ts)
		if err != nil {
			t.Errorf("Expected no error, got %v", err)
		}
		t.Logf("WriteToCache completed")

		result := &testStoreData{
			Id: "1",
		}
		if err := store.Read(result); err != nil {
			t.Errorf("failed to read back data: %v, got %v", err, result.Value)
		}
		if result.Value != "trueblocks" {
			t.Errorf("wrong value: %v, got %v", err, result.Value)
		}
	})

	t.Run("StoreDisabled", func(t *testing.T) {
		store, err := newTestStore(false, walk.Cache_Results, true, 5000)
		if err != nil {
			t.Errorf("Failed to create store %v", err)
		}

		data := &testStoreData{Id: "2", Value: "test"}
		finalTs := base.Timestamp(900)

		err = store.WriteToCache(data, walk.Cache_Results, finalTs)
		if err != nil {
			t.Errorf("Expected nil error, got %v", err)
		}

		readBack := &testStoreData{Id: "2"}
		if err := store.Read(readBack); err == nil {
			t.Errorf("Expected read to fail (no write occurred)")
		}
	})

	t.Run("CacheTypeDisabled", func(t *testing.T) {
		store, err := newTestStore(true, walk.Cache_Results, false, 5000)
		if err != nil {
			t.Errorf("Failed to create store %v", err)
		}

		data := &testStoreData{Id: "3", Value: "test"}
		finalTs := base.Timestamp(900)

		err = store.WriteToCache(data, walk.Cache_Results, finalTs)
		if err != nil {
			t.Errorf("Expected nil error, got %v", err)
		}

		readBack := &testStoreData{Id: "3"}
		if err := store.Read(readBack); err == nil {
			t.Errorf("Expected read to fail (no write occurred)")
		}
	})

	t.Run("TimestampNotFinal", func(t *testing.T) {
		store, err := newTestStore(true, walk.Cache_Results, true, 1002)
		if err != nil {
			t.Errorf("Failed to create store %v", err)
		}
		data := &testStoreData{Id: "4", Value: "test"}
		nonFinalTs := base.Timestamp(1001)

		err = store.WriteToCache(data, walk.Cache_Results, nonFinalTs)
		if err != nil {
			t.Errorf("Expected nil error, got %v", err)
		}

		readBack := &testStoreData{Id: "4"}
		if err := store.Read(readBack); err == nil {
			t.Errorf("Expected read to fail (no write occurred)")
		}
	})

	t.Run("AdditionalConditionFails", func(t *testing.T) {
		store, err := newTestStore(true, walk.Cache_Statements, true, 5000)
		if err != nil {
			t.Errorf("Failed to create store %v", err)
		}
		data := &testStoreData{Id: "5", Value: "test"}
		finalTs := base.Timestamp(900)
		allReconciled := false

		err = store.WriteToCache(data, walk.Cache_Statements, finalTs, allReconciled)
		if err != nil {
			t.Errorf("Expected nil error, got %v", err)
		}

		readBack := &testStoreData{Id: "5"}
		if err := store.Read(readBack); err == nil {
			t.Errorf("Expected read to fail (no write occurred)")
		}
	})

	t.Run("WriteFails", func(t *testing.T) {
		store, err := newTestStore(true, walk.Cache_Results, true, 5000)
		if err != nil {
			t.Errorf("Failed to create store %v", err)
		}
		invalidData := &testStoreData{Id: "", Value: "test"}
		finalTs := base.Timestamp(900)

		err = store.WriteToCache(invalidData, walk.Cache_Results, finalTs)
		if err != nil {
			fmt.Println(err.Error())
		}
		if err == nil || !strings.Contains(err.Error(), "failed to write results to cache: empty CacheLocations") {
			t.Errorf("Expected wrapped error, got %v", err)
		}
	})

	t.Run("NilStore", func(t *testing.T) {
		var nilStore *Store
		data := &testStoreData{Id: "6", Value: "test"}
		finalTs := base.Timestamp(900)

		err := nilStore.WriteToCache(data, walk.Cache_Results, finalTs)
		if err != nil {
			t.Errorf("Expected nil error, got %v", err)
		}
	})

	t.Run("InvalidCacheType", func(t *testing.T) {
		store, err := newTestStore(true, walk.CacheType(999), false, 5000)
		if err != nil {
			t.Errorf("Failed to create store %v", err)
		}
		data := &testStoreData{Id: "7", Value: "test"}
		finalTs := base.Timestamp(900)

		err = store.WriteToCache(data, walk.CacheType(999), finalTs)
		if err != nil {
			t.Errorf("Expected nil error, got %v", err)
		}

		readBack := &testStoreData{Id: "7"}
		if err := store.Read(readBack); err == nil {
			t.Errorf("Expected read to fail (no write occurred)")
		}
	})
}

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

func newTestStore(enabled bool, cacheType walk.CacheType, enabledMapValue bool, latest base.Timestamp) (*Store, error) {
	store, err := NewStore(&StoreOptions{Location: MemoryCache})
	store.enabled = enabled
	store.enabledMap = map[walk.CacheType]bool{cacheType: enabledMapValue}
	store.latest = latest
	if err != nil {
		return nil, err
	}
	return store, nil
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
