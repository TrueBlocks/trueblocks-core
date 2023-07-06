package binary

import (
	"bytes"
	"encoding/binary"
	"io"
	"reflect"
	"strconv"
	"testing"
)

type testUnmarshaler struct {
	value int32
}

func (t *testUnmarshaler) UnmarshalCache(version uint64, reader io.Reader) error {
	return binary.Read(reader, binary.LittleEndian, &t.value)
}

type likeString string

func (l *likeString) UnmarshalCache(version uint64, reader io.Reader) error {
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
}
