package base

import (
	"bytes"
	"encoding/binary"
	"io"
	"math/big"
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

	buf.Reset()
	expectedMarshalerSlice := []Marshaler{
		&testMarshaler{"10"},
		&testMarshaler{"21"},
	}
	if err := WriteValue(buf, expectedMarshalerSlice); err != nil {
		t.Fatal(err)
	}
	if err := binary.Read(buf, binary.LittleEndian, &sliceSize); err != nil {
		t.Fatal(err)
	}
	marshalerSliceValue := make([]int32, 0, sliceSize)
	for range expectedMarshalerSlice {
		var value int32
		if err := binary.Read(buf, binary.LittleEndian, &value); err != nil {
			t.Fatal(err)
		}
		marshalerSliceValue = append(marshalerSliceValue, value)
	}
	if len(marshalerSliceValue) != len(expectedMarshalerSlice) {
		t.Fatal("marshaler slice wrong len")
	}
	if !reflect.DeepEqual(marshalerSliceValue, []int32{10, 21}) {
		t.Fatal("string slice wrong value", marshalerSliceValue)
	}
}

func TestWriteBigInt(t *testing.T) {
	buf := new(bytes.Buffer)
	bigint := big.NewInt(0)
	_, ok := bigint.SetString("57006123709077586392", 10)
	if !ok {
		t.Fatal("cannot set test value")
	}
	if err := writeBigInt(buf, bigint); err != nil {
		t.Fatal(err)
	}

	var size uint64
	if err := binary.Read(buf, binary.LittleEndian, &size); err != nil {
		t.Fatal(err)
	}
	// we expect size to be len(bigint.Bytes()) + 1, because GobEncode adds 1 byte
	// for storing the sign
	if byteLen := len(bigint.Bytes()) + 1; int(size) != byteLen {
		t.Fatal("size is", size, "expected", byteLen)
	}

	data := make([]byte, size)
	if err := binary.Read(buf, binary.LittleEndian, data); err != nil {
		t.Fatal(err)
	}

	result := big.NewInt(0)
	if err := result.GobDecode(data); err != nil {
		t.Fatal(err)
	}
	if result.Cmp(bigint) != 0 {
		t.Fatal("values are not same")
	}
}
