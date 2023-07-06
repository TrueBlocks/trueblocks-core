package binary

import (
	"bytes"
	"encoding/binary"
	"io"
	"reflect"
	"strconv"
	"testing"
)

type testMarshaler struct {
	value string
}

func (t *testMarshaler) MarshalCache(writer io.Writer) error {
	v, err := strconv.Atoi(t.value)
	if err != nil {
		return err
	}
	return binary.Write(writer, binary.LittleEndian, int32(v))
}

func TestWriteValue(t *testing.T) {
	var data []byte
	buf := bytes.NewBuffer(data)

	expectedUint64 := uint64(123)
	if err := WriteValue(buf, expectedUint64); err != nil {
		t.Fatal(err)
	}
	var uint64Value uint64
	if err := binary.Read(buf, binary.LittleEndian, &uint64Value); err != nil {
		t.Fatal(err)
	}

	if uint64Value != expectedUint64 {
		t.Fatal("uint64 wrong value:", uint64Value)
	}

	buf.Reset()
	expectedStr := "hello"
	expectedStrLen := uint64(len(expectedStr))
	if err := WriteValue(buf, expectedStr); err != nil {
		t.Fatal(err)
	}
	var stringValue string
	var stringLen uint64
	var strBytes []byte
	if err := binary.Read(buf, binary.LittleEndian, &stringLen); err != nil {
		t.Fatal(err)
	}
	strBytes = make([]byte, stringLen)
	if err := binary.Read(buf, binary.LittleEndian, strBytes); err != nil {
		t.Fatal(err)
	}
	stringValue = string(strBytes)
	if stringLen != expectedStrLen {
		t.Fatal("string wrong length", stringLen)
	}
	if stringValue != expectedStr {
		t.Fatal("string wrong value:", stringValue)
	}

	buf.Reset()
	expectedStrSlice := []string{"hello", "world"}
	if err := WriteValue(buf, expectedStrSlice); err != nil {
		t.Fatal(err)
	}
	var sliceSize uint64
	if err := binary.Read(buf, binary.LittleEndian, &sliceSize); err != nil {
		t.Fatal(err)
	}
	stringSliceValue := make([]string, 0, sliceSize)
	for range expectedStrSlice {
		var strLen uint64
		if err := binary.Read(buf, binary.LittleEndian, &strLen); err != nil {
			t.Fatal(err)
		}
		value := make([]byte, 5)
		if err := binary.Read(buf, binary.LittleEndian, value); err != nil {
			t.Fatal(err)
		}
		stringSliceValue = append(stringSliceValue, string(value))
	}
	if !reflect.DeepEqual(stringSliceValue, expectedStrSlice) {
		t.Fatal("string slice wrong value", stringSliceValue)
	}

	buf.Reset()
	marshalValue := new(testMarshaler)
	marshalValue.value = "42"
	expectedMarshalValue := int32(42)
	if err := WriteValue(buf, marshalValue); err != nil {
		t.Fatal(err)
	}
	var marshalResult int32
	if err := binary.Read(buf, binary.LittleEndian, &marshalResult); err != nil {
		t.Fatal(err)
	}
	if marshalResult != expectedMarshalValue {
		t.Fatal("unmarshal wrong value", marshalResult)
	}
}
