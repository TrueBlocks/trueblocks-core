package base

import (
	"encoding/binary"
	"errors"
	"io"
	"math/big"
	"reflect"
)

type Unmarshaler interface {
	UnmarshalCache(vers uint64, reader io.Reader) error
}

// ReadValue reads binary representation of fixed-size values, strings,
// big.Int, Unmarshaler and slices of these values. Version number
// is passed to any Unmarshaler to ease reading older formats.
func ReadValue(reader io.Reader, dest any, fileVersion uint64) (err error) {
	switch d := dest.(type) {
	case Unmarshaler:
		err = d.UnmarshalCache(fileVersion, reader)
	case *[]string:
		err = readSlice(reader, d, fileVersion)
	case *[]big.Int:
		err = readSlice(reader, d, fileVersion)
	case *string:
		err = readString(reader, d)
	case *big.Int:
		err = readBigInt(reader, d)
	case *big.Float:
		err = readBigFloat(reader, d)
	default:
		// Reading []Unmarshaler is a bit more complex. The type switch won't work and
		// we'll end up here. If value is a pointer to a slice, then it may contain Unmarshalers
		reflectedValue := reflect.ValueOf(dest)
		if reflectedValue.Kind() == reflect.Pointer {
			if reflectedValue.Elem().Kind() == reflect.Slice {
				// It is what we want, so let's try to read. If we get an error, we'll ignore it and still
				err = readSliceReflect(reader, reflect.TypeOf(dest).Elem(), reflectedValue.Elem(), fileVersion)
				if err == nil {
					return
				}
			}
		}
		err = binary.Read(reader, binary.LittleEndian, dest)
	}
	return err
}

// readSlice reads binary representation of slice of T. ReadValue is called for each
// item, so slice items can be of any type supported by ReadValue.
func readSlice[T any](reader io.Reader, slice *[]T, fileVersion uint64) (err error) {
	var itemCount uint64 = 0
	if err = binary.Read(reader, binary.LittleEndian, &itemCount); err != nil {
		return
	}

	// make target large enough
	*slice = make([]T, 0, itemCount)
	for i := 0; uint64(i) < itemCount; i++ {
		item := new(T)
		if err = ReadValue(reader, item, fileVersion); err != nil {
			return
		}

		*slice = append(*slice, *item)
	}
	return nil
}

// readSliceReflect uses reflection to read a slice (typically of Unmarshaler)
func readSliceReflect(reader io.Reader, slice reflect.Type, destPointer reflect.Value, fileVersion uint64) (err error) {
	var itemCount uint64 = 0
	if err = binary.Read(reader, binary.LittleEndian, &itemCount); err != nil {
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
			if err = ReadValue(reader, item.Interface(), fileVersion); err != nil {
				return
			}
		} else {
			if err = ReadValue(reader, unmarshaler, fileVersion); err != nil {
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
	if err = binary.Read(reader, binary.LittleEndian, &size); err != nil {
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
	if err = binary.Read(reader, binary.LittleEndian, &size); err != nil {
		return
	}

	// now we read as many bytes as we need
	data := make([]byte, size)
	if err = binary.Read(reader, binary.LittleEndian, data); err != nil {
		return
	}
	// and use big.Int.GobDecode to decode the value
	err = target.GobDecode(data)
	return
}

func readBigFloat(reader io.Reader, dest *big.Float) (err error) {
	var length uint64
	if err = binary.Read(reader, binary.LittleEndian, &length); err != nil {
		return err
	}
	data := make([]byte, length)
	if _, err = io.ReadFull(reader, data); err != nil {
		return err
	}
	return dest.GobDecode(data)
}
