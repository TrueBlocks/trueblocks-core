package binary

import (
	"encoding/binary"
	"io"
	"math/big"
)

// read reads bytes in a correct byte order
func read(reader io.Reader, value any) (err error) {
	return binary.Read(reader, binary.LittleEndian, value)
}

// ReadValue reads binary representation of fixed-size values, strings,
// big.Int, CacheUnmarshaler and slices of these values. Version number
// is passed to any CacheUnmarshaler to ease reading older formats.
func ReadValue(reader io.Reader, value any, version uint64) (err error) {
	unmarshaler, ok := value.(CacheUnmarshaler)
	if ok {
		// TODO: fix version
		return unmarshaler.UnmarshalCache(uint64(41000), reader)
	}

	// binary.Read takes care of slices of fixed-size types, e.g. []uint8,
	// so we only have to support []string, []big.Int and []CacheUnmarshaler
	strSlice, ok := any(value).(*[]string)
	if ok {
		return ReadSlice(reader, strSlice, version)
	}
	bigSlice, ok := any(value).(*[]big.Int)
	if ok {
		return ReadSlice(reader, bigSlice, version)
	}
	cacheMarshalerSlice, ok := any(value).(*[]CacheUnmarshaler)
	if ok {
		return ReadSlice(reader, cacheMarshalerSlice, version)
	}

	str, ok := any(value).(*string)
	if ok {
		return ReadString(reader, str)
	}

	bigint, ok := any(value).(*big.Int)
	if ok {
		return ReadBigInt(reader, bigint)
	}

	return read(reader, value)
}

// ReadSlice reads binary representation of slice of T. ReadValue is called for each
// item, so slice items can be of any type supported by ReadValue.
func ReadSlice[T any](reader io.Reader, slice *[]T, version uint64) (err error) {
	var itemCount uint64 = 0
	if err = read(reader, &itemCount); err != nil {
		return
	}

	// make target large enough
	*slice = make([]T, 0, itemCount)
	for i := 0; uint64(i) < itemCount; i++ {
		item := new(T)
		if err = ReadValue(reader, item, version); err != nil {
			return
		}

		*slice = append(*slice, *item)
	}
	return nil
}

func ReadString(reader io.Reader, str *string) (err error) {
	var size uint64
	if err = read(reader, &size); err != nil {
		return
	}

	content := make([]byte, size)
	if err = ReadValue(reader, content, 0); err != nil {
		return err
	}
	*str = string(content)
	return
}

func ReadBigInt(reader io.Reader, target *big.Int) (err error) {
	descriptor := struct{ Capacity, Length int32 }{}
	// read descriptor
	if err = read(reader, &descriptor); err != nil {
		return
	}

	if descriptor.Length == 0 {
		*target = *big.NewInt(0)
		return
	}

	// read items as slice of bytes
	items := make([]byte, descriptor.Length*8)
	if err = read(reader, &items); err != nil {
		return
	}
	result := big.NewInt(0)
	// in order to get the correct number, we have to manually reverse bytes
	// see https://github.com/golang/go/issues/40891
	orderedItems := reverseBytes(items)
	// load bytes into big.Int
	result.SetBytes(orderedItems)
	*target = *result
	return
}
