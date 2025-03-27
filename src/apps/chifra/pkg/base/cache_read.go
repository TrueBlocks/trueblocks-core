package base

import (
	"encoding/binary"
	"errors"
	"io"
	"math/big"
	"reflect"
)

// Unmarshaler is a struct implementing Unmarshaler can be read from binary by
// calling UnmarshalCache
type Unmarshaler interface {
	UnmarshalCache(vers uint64, reader io.Reader) error
}

// Marshaler is a struct implementing the Marshaler interface. It can be
// written to binary by calling MarshalCache
type Marshaler interface {
	MarshalCache(writer io.Writer) error
}

// read reads bytes in a correct byte order
func read(reader io.Reader, value any) (err error) {
	return binary.Read(reader, binary.LittleEndian, value)
}

// ReadValue reads binary representation of fixed-size values, strings,
// big.Int, Unmarshaler and slices of these values. Version number
// is passed to any Unmarshaler to ease reading older formats.
func ReadValue(reader io.Reader, value any, version uint64) (err error) {
	switch v := value.(type) {
	case Unmarshaler:
		err = v.UnmarshalCache(version, reader)
	case *[]string:
		err = readSlice(reader, v, version)
	case *[]big.Int:
		err = readSlice(reader, v, version)
	case *string:
		err = readString(reader, v)
	case *big.Int:
		err = readBigInt(reader, v)
	default:
		// Reading []Unmarshaler is a bit more complex. The type switch won't work and
		// we'll end up here. If value is a pointer to a slice, then it may contain Unmarshalers
		reflectedValue := reflect.ValueOf(value)
		if reflectedValue.Kind() == reflect.Pointer {
			if reflectedValue.Elem().Kind() == reflect.Slice {
				// It is what we want, so let's try to read. If we get an error, we'll ignore it and still
				err = readSliceReflect(reader, reflect.TypeOf(value).Elem(), reflectedValue.Elem(), version)
				if err == nil {
					return
				}
			}
		}

		err = read(reader, value)
	}
	return
}

// readSlice reads binary representation of slice of T. ReadValue is called for each
// item, so slice items can be of any type supported by ReadValue.
func readSlice[T any](reader io.Reader, slice *[]T, version uint64) (err error) {
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

// readSliceReflect uses reflection to read a slice (typically of Unmarshaler)
func readSliceReflect(reader io.Reader, slice reflect.Type, destPointer reflect.Value, version uint64) (err error) {
	var itemCount uint64 = 0
	if err = read(reader, &itemCount); err != nil {
		return
	}

	sliceItemType := slice.Elem()
	if !reflect.New(sliceItemType).CanInterface() {
		return errors.New("cannot cast value to interface")
	}
	sliceOfType := reflect.SliceOf(sliceItemType)

	// Make sure we return a correct zero value for empty arrays
	if itemCount == 0 {
		destPointer.Set(reflect.Zero(sliceOfType))
		return
	}

	result := reflect.MakeSlice(sliceOfType, 0, int(itemCount))
	for i := 0; uint64(i) < itemCount; i++ {
		item := reflect.New(sliceItemType)

		unmarshaler, ok := item.Interface().(Unmarshaler)
		if !ok {
			// If it's not Unmarshaler, it can be a simpler type, which we can read into
			if err = ReadValue(reader, item.Interface(), version); err != nil {
				return
			}
		} else {
			if err = ReadValue(reader, unmarshaler, version); err != nil {
				return
			}
		}
		result = reflect.Append(result, item.Elem())
	}

	destPointer.Set(result)

	return nil
}

func readString(reader io.Reader, str *string) (err error) {
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

func readBigInt(reader io.Reader, target *big.Int) (err error) {
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
