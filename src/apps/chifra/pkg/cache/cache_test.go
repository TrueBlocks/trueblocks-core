package cache

import (
	"bytes"
	"errors"
	"fmt"
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/binary"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

// Here we define a struct that we want to write or read from the cache
type ExampleBlock struct {
	BlockNumber uint64
	Date        string
	Name        string
	Timestamp   uint64
	raw         *RawExampleBlock
}

// This is a "raw" version of the data, just like in our types package
type RawExampleBlock struct {
	BlockNumber uint64
}

// We now make ExampleBlock implement CacheMarshaler interface, by
// defining MarshalCache. MarshalCache job is to serialize the data more
// complex than uint8, uint64, ..., int64, etc.
func (e *ExampleBlock) MarshalCache(writer io.Writer) (err error) {
	// binary.WriteValue serializes its second argument. It works for
	// fixed-size numeric types, slices and structs that implement
	// CacheMarshaler interface (and slices of such types).
	if err = binary.WriteValue(writer, e.BlockNumber); err != nil {
		return err
	}
	if err = binary.WriteValue(writer, e.Name); err != nil {
		return err
	}
	if err = binary.WriteValue(writer, e.Timestamp); err != nil {
		return err
	}
	// We are not saving e.raw, although it's possible

	return
}

var minimalVersion version.Version

func init() {
	var err error
	minimalVersion, err = version.NewVersion("GHC-TrueBlocks//0.10.0-beta")
	if err != nil {
		panic(err)
	}
}

// Now we make ExampleBlock implement CacheUnmarshaler interface, which is like
// CacheMarshaler, but for reading
func (e *ExampleBlock) UnmarshalCache(itemVersion uint64, reader io.Reader) (err error) {
	// We will see where version is extracted in Example() below
	if itemVersion < minimalVersion.Uint64() {
		// In real life we would read back level
		return errors.New("unsupported version")
	}

	if err = binary.ReadValue(reader, &e.BlockNumber, itemVersion); err != nil {
		return err
	}
	if err = binary.ReadValue(reader, &e.Name, itemVersion); err != nil {
		return err
	}
	if err = binary.ReadValue(reader, &e.Timestamp, itemVersion); err != nil {
		return err
	}
	return
}

func Example() {
	// We start with a simple example of storing uint64 serialized to binary

	// First, we create a fake file (we need something that implements io.ReadWriter)
	timestampsCacheFile := new(bytes.Buffer)
	// Now we point our cache item to use this "file". This is temporary, it will be
	// handled by some higher abstraction (CacheLayout perhaps?)
	timestampsCacheItem := NewItem(timestampsCacheFile)
	// We encode (serialize to binary) the value. The cache header is written automaticaly
	// by Encode()
	if err := timestampsCacheItem.Encode(uint64(1688667358)); err != nil {
		panic(err)
	}

	// Let's read the value
	var timestamp uint64
	// Decode checks cache header automaticaly
	if err := timestampsCacheItem.Decode(&timestamp); err != nil {
		panic(err)
	}
	fmt.Println(timestamp)

	// Now we will write a structer which implements Cache(un)Marshaler

	rawBlock := &RawExampleBlock{
		BlockNumber: 17636511,
	}
	block := &ExampleBlock{
		BlockNumber: rawBlock.BlockNumber,
		Date:        "2023-07-06",
		Name:        "Nice Block",
		Timestamp:   1688667358,
	}

	// Again, we create a fake file and cache item linked to it. And again, it'll be
	// more automated later
	blocksCacheFile := new(bytes.Buffer)
	blocksCacheItem := NewItem(blocksCacheFile)
	// We call Encode() to serialize to binary. Cache header is written by default
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
	// &{BlockNumber:17636511 Date: Name:Nice Block Timestamp:1688667358 raw:<nil>}
}
