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
	switch v := value.(type) {
	case CacheUnmarshaler:
		err = v.UnmarshalCache(version, reader)

	// binary.Read takes care of slices of fixed-size types, e.g. []uint8,
	// so we only have to support []string, []big.Int and []CacheUnmarshaler
	case *[]string:
		err = ReadSlice(reader, v, version)
	case *[]big.Int:
		err = ReadSlice(reader, v, version)
	case *[]CacheUnmarshaler:
		err = ReadSlice(reader, v, version)
	case *string:
		err = ReadString(reader, v)
	case *big.Int:
		err = ReadBigInt(reader, v)
	default:
		err = read(reader, value)
	}
	return
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
	// we first need to learn how many bytes we should read
	// (GobEncode that we use to write big.Int produces
	// variable-length []byte)
	var size uint64
	if err = read(reader, &size); err != nil {
		return
	}

	// now we read as many bytes as we need
	data := make([]byte, size)
	if err = read(reader, data); err != nil {
		return
	}
	// and use big.Int.GobDecode to decode the value
	err = target.GobDecode(data)
	return
}
