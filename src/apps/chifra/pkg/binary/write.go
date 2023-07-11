package binary

import (
	"bytes"
	"encoding/binary"
	"io"
	"math/big"
)

// write writes bytes in a correct byte order
func write(writer io.Writer, value any) (err error) {
	return binary.Write(writer, binary.LittleEndian, value)
}

// WriteValue writes binary representation of fixed-size values, strings,
// big.Int, CacheUnmarshaler and slices of these values. Contrary to
// ReadValue, it doesn't support versioning, because it is expected that
// only the most recent data format is used when writing.
func WriteValue(writer io.Writer, value any) (err error) {
	switch v := value.(type) {
	case CacheMarshaler:
		err = v.MarshalCache(writer)

	// binary.Write takes care of slices of fixed-size types, e.g. []uint8,
	// so we only have to support []string, []big.Int and []CacheUnmarshaler
	case []string:
		err = WriteSlice(writer, v)
	case []big.Int:
		err = WriteSlice(writer, v)
	case []CacheMarshaler:
		err = WriteSlice(writer, v)
	case string:
		err = WriteString(writer, &v)
	case *big.Int:
		err = WriteBigInt(writer, v)
	default:
		err = write(writer, value)
	}
	return err
}

// WriteSlice reads binary representation of slice of T. WriteValue is called for each
// item, so slice items can be of any type supported by WriteValue.
func WriteSlice[T any](writer io.Writer, slice []T) (err error) {
	buffer := new(bytes.Buffer)
	sliceLen := uint64(len(slice))

	if err = write(buffer, sliceLen); err != nil {
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

func WriteString(writer io.Writer, str *string) (err error) {
	if err = write(writer, uint64(len(*str))); err != nil {
		return
	}
	return write(writer, []byte(*str))
}

func WriteBigInt(writer io.Writer, value *big.Int) (err error) {
	data, err := value.GobEncode()
	if err != nil {
		return
	}
	// write length of data, so ReadBigInt knows how many bytes to read
	if err = write(writer, uint64(len(data))); err != nil {
		return err
	}

	// write the data
	_, err = writer.Write(data)
	return
}
