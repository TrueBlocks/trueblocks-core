package base

import (
	"bytes"
	"encoding/binary"
	"io"
	"math/big"
	"reflect"
)

type Marshaler interface {
	MarshalCache(writer io.Writer) error
}

// WriteValue writes binary representation of fixed-size values, strings,
// big.Int, Marshaler and slices of these values. Contrary to
// ReadValue, it doesn't support versioning, because it is expected that
// only the most recent data format is used when writing.
func WriteValue(writer io.Writer, value any) (err error) {
	switch v := value.(type) {
	case Marshaler:
		err = v.MarshalCache(writer)

	// binary.Write takes care of slices of fixed-size types, e.g. []uint8,
	// so we only have to support []string, []big.Int and []Marshaler
	case []string:
		err = writeSlice(writer, v)
	case []big.Int:
		err = writeSlice(writer, v)
	case []Marshaler:
		err = writeSlice(writer, v)
	case string:
		err = writeString(writer, &v)
	case *big.Int:
		err = writeBigInt(writer, v)
	case *big.Float:
		err = writeBigFloat(writer, v)
	default:
		if rf := reflect.ValueOf(value); rf.Kind() == reflect.Slice {
			return writeSliceReflect(writer, &rf)
		}
		err = binary.Write(writer, binary.LittleEndian, value)
	}
	return err
}

// writeSlice reads binary representation of slice of T. WriteValue is called for each
// item, so slice items can be of any type supported by WriteValue.
func writeSlice[T any](writer io.Writer, slice []T) (err error) {
	buffer := new(bytes.Buffer)
	sliceLen := uint64(len(slice))

	if err = binary.Write(buffer, binary.LittleEndian, sliceLen); err != nil {
		return
	}
	for _, sliceItem := range slice {
		if err = WriteValue(buffer, sliceItem); err != nil {
			return
		}
	}
	if _, err = buffer.WriteTo(writer); err != nil {
		return err
	}

	return
}

func writeSliceReflect(writer io.Writer, sliceValue *reflect.Value) (err error) {
	buffer := new(bytes.Buffer)
	sliceLen := sliceValue.Len()

	if err = binary.Write(buffer, binary.LittleEndian, uint64(sliceLen)); err != nil {
		return
	}
	for i := 0; i < sliceLen; i++ {
		item := sliceValue.Index(i)

		cm, ok := item.Addr().Interface().(Marshaler)
		if !ok {
			if err = WriteValue(buffer, item.Interface()); err != nil {
				return
			}
		} else {
			if err = WriteValue(buffer, cm); err != nil {
				return
			}
		}
	}
	if _, err = buffer.WriteTo(writer); err != nil {
		return err
	}

	return
}

func writeString(writer io.Writer, str *string) (err error) {
	if err = binary.Write(writer, binary.LittleEndian, uint64(len(*str))); err != nil {
		return
	}
	return binary.Write(writer, binary.LittleEndian, []byte(*str))
}

func writeBigInt(writer io.Writer, value *big.Int) (err error) {
	data, err := value.GobEncode()
	if err != nil {
		return
	}
	// write length of data, so readBigInt knows how many bytes to read
	if err = binary.Write(writer, binary.LittleEndian, uint64(len(data))); err != nil {
		return err
	}

	// write the data
	_, err = writer.Write(data)
	return
}

func writeBigFloat(writer io.Writer, value *big.Float) (err error) {
	data, err := value.GobEncode()
	if err != nil {
		return err
	}
	if err = binary.Write(writer, binary.LittleEndian, uint64(len(data))); err != nil {
		return err
	}
	_, err = writer.Write(data)
	return err
}
