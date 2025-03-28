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

type testUnmarshaler struct {
	value int32
}

func (t *testUnmarshaler) UnmarshalCache(fileVersion uint64, reader io.Reader) error {
	return binary.Read(reader, binary.LittleEndian, &t.value)
}

type likeString string

func (l *likeString) UnmarshalCache(fileVersion uint64, reader io.Reader) error {
	var v int32
	if err := binary.Read(reader, binary.LittleEndian, &v); err != nil {
		return err
	}
	s := strconv.Itoa(int(v))
	*l = likeString(s)
	return nil
}

func TestReadValue(t *testing.T) {
	var data []byte
	buf := bytes.NewBuffer(data)

	expectedUint64 := uint64(123)
	if err := binary.Write(buf, binary.LittleEndian, expectedUint64); err != nil {
		t.Fatal(err)
	}
	var uint64Value uint64
	if err := ReadValue(buf, &uint64Value, 0); err != nil {
		t.Fatal(err)
	}
	if uint64Value != expectedUint64 {
		t.Fatal("uint64 wrong value:", uint64Value)
	}

	buf.Reset()
	expectedStr := "hello"
	binaryStrLen := uint64(len(expectedStr))
	binaryStr := []byte(expectedStr)
	if err := binary.Write(buf, binary.LittleEndian, binaryStrLen); err != nil {
		t.Fatal(err)
	}
	if err := binary.Write(buf, binary.LittleEndian, binaryStr); err != nil {
		t.Fatal(err)
	}
	var stringValue string
	if err := ReadValue(buf, &stringValue, 0); err != nil {
		t.Fatal(err)
	}
	if stringValue != expectedStr {
		t.Fatal("string wrong value:", stringValue)
	}

	buf.Reset()
	expectedStrSlice := []string{"hello", "world"}
	if err := binary.Write(buf, binary.LittleEndian, uint64(2)); err != nil {
		t.Fatal(err)
	}
	for _, str := range expectedStrSlice {
		if err := binary.Write(buf, binary.LittleEndian, uint64(len(str))); err != nil {
			t.Fatal(err)
		}
		if err := binary.Write(buf, binary.LittleEndian, []byte(str)); err != nil {
			t.Fatal(err)
		}
	}
	var stringSliceValue []string
	if err := ReadValue(buf, &stringSliceValue, 0); err != nil {
		t.Fatal(err)
	}
	if !reflect.DeepEqual(stringSliceValue, expectedStrSlice) {
		t.Fatal("string slice wrong value", stringSliceValue)
	}

	buf.Reset()
	expectedUnmarshalValue := int32(42)
	if err := binary.Write(buf, binary.LittleEndian, expectedUnmarshalValue); err != nil {
		t.Fatal(err)
	}
	unmarshalValue := new(testUnmarshaler)
	if err := ReadValue(buf, unmarshalValue, 0); err != nil {
		t.Fatal(err)
	}
	if unmarshalValue.value != expectedUnmarshalValue {
		t.Fatal("unmarshal wrong value", unmarshalValue.value)
	}

	expectedIntSliceValue := []int32{42, 10}
	if err := binary.Write(buf, binary.LittleEndian, uint64(len(expectedIntSliceValue))); err != nil {
		t.Fatal(err)
	}
	for _, item := range expectedIntSliceValue {
		if err := binary.Write(buf, binary.LittleEndian, item); err != nil {
			t.Fatal(err)
		}
	}
	intSliceValue := make([]int32, 0)
	if err := ReadValue(buf, &intSliceValue, 0); err != nil {
		t.Fatal(err)
	}
	if !reflect.DeepEqual(intSliceValue, expectedIntSliceValue) {
		t.Fatal("unmarshal wrong int slice value", intSliceValue)
	}

	// This test checks the use of CacheUnmarshaler being implement by a custom string type.
	// We will write int32 to the buffer. If the support for CacheUnmarshaler doesn't work, we would get an
	// error (we cannot read int32 into a string). But if it does work, UnmarshalCache will be called, intercept
	// the value and parse it into a string and set the string as the value for `likeStringValue`.
	buf.Reset()
	expectedLikeStringValue := "42"
	if err := binary.Write(buf, binary.LittleEndian, int32(42)); err != nil {
		t.Fatal(err)
	}
	var likeStringValue likeString
	if err := ReadValue(buf, &likeStringValue, 0); err != nil {
		t.Fatal(err)
	}
	if likeStringValue != likeString(expectedLikeStringValue) {
		t.Fatal("unmarshal wrong value", unmarshalValue.value)
	}

	buf.Reset()
	expectedUnmarshalSlice := []testUnmarshaler{
		{value: 42},
		{value: 10},
	}
	if err := binary.Write(buf, binary.LittleEndian, uint64(len(expectedUnmarshalSlice))); err != nil {
		t.Fatal(err)
	}
	for _, item := range expectedUnmarshalSlice {
		if err := binary.Write(buf, binary.LittleEndian, item.value); err != nil {
			t.Fatal(err)
		}
	}
	unmarshalSliceValue := make([]testUnmarshaler, 0)
	if err := ReadValue(buf, &unmarshalSliceValue, 0); err != nil {
		t.Fatal(err)
	}
	// e := []testUnmarshaler{
	// 	{value: 42},
	// 	{value: 10},
	// }
	if !reflect.DeepEqual(expectedUnmarshalSlice, unmarshalSliceValue) {
		t.Fatal("unmarshal wrong value", unmarshalSliceValue)
	}
}

func TestReadBigInt(t *testing.T) {
	buf := new(bytes.Buffer)
	bigint := big.NewInt(0)
	_, ok := bigint.SetString("57006123709077586392", 10)
	if !ok {
		t.Fatal("cannot set test value")
	}

	data, err := bigint.GobEncode()
	if err != nil {
		t.Fatal(err)
	}
	size := uint64(len(data))
	if err := binary.Write(buf, binary.LittleEndian, size); err != nil {
		t.Fatal(err)
	}
	if err := binary.Write(buf, binary.LittleEndian, data); err != nil {
		t.Fatal(err)
	}

	result := big.NewInt(0)
	if err := readBigInt(buf, result); err != nil {
		t.Fatal(err)
	}

	// we expect size to be len(bigint.Bytes()) + 1, because GobEncode adds 1 byte
	// for storing the sign
	if byteLen := len(result.Bytes()) + 1; int(size) != byteLen {
		t.Fatal("size is", size, "expected", byteLen)
	}

	if result.Cmp(bigint) != 0 {
		t.Fatal("values are not same")
	}
}

func TestReadBigFloat(t *testing.T) {
	buf := new(bytes.Buffer)
	bigfloat := big.NewFloat(0)
	if _, ok := bigfloat.SetString("123.456"); !ok {
		t.Fatal("cannot set test value")
	}

	data, err := bigfloat.GobEncode()
	if err != nil {
		t.Fatal(err)
	}
	size := uint64(len(data))
	if err := binary.Write(buf, binary.LittleEndian, size); err != nil {
		t.Fatal(err)
	}
	if err := binary.Write(buf, binary.LittleEndian, data); err != nil {
		t.Fatal(err)
	}

	result := new(big.Float)
	if err := readBigFloat(buf, result); err != nil {
		t.Fatal(err)
	}

	if result.Cmp(bigfloat) != 0 {
		t.Fatal("values are not same")
	}
}
