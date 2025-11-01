package cache

import (
	"bytes"
	"errors"
	"fmt"
	"io"
	"reflect"
	"strconv"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/version"
)

// Here we define a struct that we want to write or read from the cache
type ExampleBlock struct {
	BlockNumber uint64
	Date        string
	Name        string
	Timestamp   uint64
}

// We now make ExampleBlock implement CacheMarshaler interface, by
// defining MarshalCache. MarshalCache job is to serialize the data more
// complex than uint8, uint64, ..., int64, etc.
func (e *ExampleBlock) MarshalCache(writer io.Writer) (err error) {
	// WriteValue serializes its second argument. It works for
	// fixed-size numeric types, slices and structs that implement
	// CacheMarshaler interface (and slices of such types).
	if err = base.WriteValue(writer, e.BlockNumber); err != nil {
		return err
	}
	if err = base.WriteValue(writer, e.Name); err != nil {
		return err
	}
	if err = base.WriteValue(writer, e.Timestamp); err != nil {
		return err
	}

	return
}

var minimalVersion version.Version

func init() {
	minimalVersion = version.NewVersion("GHC-TrueBlocks//0.10.0-beta")
}

// Now we make ExampleBlock implement CacheUnmarshaler interface, which is like
// CacheMarshaler, but for reading
func (e *ExampleBlock) UnmarshalCache(itemVersion uint64, reader io.Reader) (err error) {
	// We will see where version is extracted in Example() below
	if itemVersion < minimalVersion.Uint64() {
		// In real life we would read back level
		return errors.New("unsupported version")
	}

	if err = base.ReadValue(reader, &e.BlockNumber, itemVersion); err != nil {
		return err
	}
	if err = base.ReadValue(reader, &e.Name, itemVersion); err != nil {
		return err
	}
	if err = base.ReadValue(reader, &e.Timestamp, itemVersion); err != nil {
		return err
	}
	return
}

// We we also implement all the methods needed for ExampleBlock to become Locator.
// Locator interface job is to locate an item in cache.

func (e *ExampleBlock) CacheLocations() (string, string, string) {
	return "example", strconv.FormatUint(e.BlockNumber, 10), "bin"
}

func Example() {
	// This example shows the usage of Locator interface, which is higher level
	// abstraction over Cache(un)Marshaler

	// Let's create something to store
	block := &ExampleBlock{
		BlockNumber: 4436721,
		Name:        "Test block",
		Timestamp:   1688667358,
	}

	// We made `BlockNumber` our cache ID
	_, id, _ := block.CacheLocations()
	fmt.Println(id)

	// we will store it in memory cache (if StoreOptions is nil, then file system
	// cache is used)
	store, err := NewStore(&StoreOptions{Location: MemoryCache})
	if err != nil {
		panic(err)
	}

	if err := store.Write(block); err != nil {
		panic(err)
	}

	readFromCache := &ExampleBlock{
		BlockNumber: 4436721,
	}
	if err := store.ReadFromStore(readFromCache); err != nil {
		panic(err)
	}

	fmt.Printf("%+v\n", readFromCache)

	// Output:
	// 4436721
	// &{BlockNumber:4436721 Date: Name:Test block Timestamp:1688667358}
}

func ExampleMarshaler() {
	// We start with a simple example of storing uint64 serialized to binary

	// First, we create a fake file (we need something that implements io.ReadWriter)
	timestampsCacheFile := new(bytes.Buffer)
	// Now we point our cache item to use this "file". This is temporary, it will be
	// handled by some higher abstraction (CacheLayout perhaps?)
	timestampsCacheItem := NewItem(timestampsCacheFile)
	// We encode (serialize to binary) the value. The cache header is written automatically
	// by Encode()
	if err := timestampsCacheItem.Encode(uint64(1688667358)); err != nil {
		panic(err)
	}

	// Let's read the value
	var timestamp uint64
	// Decode checks cache header automatically
	if err := timestampsCacheItem.Decode(&timestamp); err != nil {
		panic(err)
	}
	fmt.Println(timestamp)

	// Now we will write a structure which implements Cache(un)Marshaler
	block := &ExampleBlock{
		BlockNumber: 17636511,
		Date:        "2023-07-06",
		Name:        "Nice Block",
		Timestamp:   1688667358,
	}

	// Again, we create a fake file and cache item linked to it. And again, it'll be
	// more automated later
	blocksCacheFile := new(bytes.Buffer)
	blocksCacheItem := NewItem(blocksCacheFile)
	// We call Encode() to serialize to  Cache header is written by default
	if err := blocksCacheItem.Encode(block); err != nil {
		panic(err)
	}

	// Now we can read the struct back
	decodedBlock := &ExampleBlock{}
	if err := blocksCacheItem.Decode(decodedBlock); err != nil {
		panic(err)
	}
	fmt.Printf("%+v\n", decodedBlock)

	// Output:
	// 1688667358
	// &{BlockNumber:17636511 Date: Name:Nice Block Timestamp:1688667358}
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
