package output

import (
	"bytes"
	"errors"
	"fmt"
	"strconv"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func helperReportStringMismatch(t *testing.T, expected, result string) {
	for i, ch := range expected {
		if result[i] != byte(ch) {
			fmt.Printf("<<%s>>", string(result[:i]))
			t.Fatalf("mismatch at %d, expected %s, got %s", i, strconv.QuoteRune(ch), strconv.Quote(string(result[i])))
		}
	}
}

func TestJsonWriter_EmptyRoot(t *testing.T) {
	expected := `{}`

	b := make([]byte, 0, 1024)
	buf := bytes.NewBuffer(b)
	w := NewJsonWriter(buf)
	w.Close()

	result := buf.String()
	if result != expected {
		helperReportStringMismatch(t, expected, result)
	}
}

func TestJsonWriter_EmptyField(t *testing.T) {
	expected := `{
  "data": []
}`

	b := make([]byte, 0, 1024)
	buf := bytes.NewBuffer(b)
	w := NewJsonWriter(buf)
	_, _ = w.OpenField("data", FieldArray)
	_, _ = w.CloseField(FieldArray)
	w.Close()

	result := buf.String()

	if result != expected {
		helperReportStringMismatch(t, expected, result)
	}
}

func TestJsonWriter_EmptySubField(t *testing.T) {
	expected := `{
  "data": [
    {}
  ]
}`

	b := make([]byte, 0, 1024)
	buf := bytes.NewBuffer(b)
	w := NewJsonWriter(buf)
	_, _ = w.OpenField("data", FieldArray)
	_, _ = w.OpenField("", FieldObject)
	_, _ = w.CloseField(FieldObject)
	_, _ = w.CloseField(FieldArray)
	w.Close()

	result := buf.String()

	if result != expected {
		helperReportStringMismatch(t, expected, result)
	}
}

func TestJsonWriter_SingleKey(t *testing.T) {
	expected := `{
  "data": [
    {
      "key": true
    }
  ]
}`

	b := make([]byte, 0, 1024)
	buf := bytes.NewBuffer(b)
	w := NewJsonWriter(buf)
	_, _ = w.OpenField("data", FieldArray)
	_, _ = w.OpenField("", FieldObject)
	_, _ = w.Write([]byte(`"key": true`))
	_, _ = w.CloseField(FieldObject)
	_, _ = w.CloseField(FieldArray)
	w.Close()

	result := buf.String()

	if result != expected {
		helperReportStringMismatch(t, expected, result)
	}
}

func TestJsonWriter_SingleEmptySubkey(t *testing.T) {
	expected := `{
  "data": [
    {
      "subkey": {}
    }
  ]
}`

	b := make([]byte, 0, 1024)
	buf := bytes.NewBuffer(b)
	w := NewJsonWriter(buf)
	_, _ = w.OpenField("data", FieldArray)
	_, _ = w.OpenField("", FieldObject)
	_, _ = w.OpenField("subkey", FieldObject)
	_, _ = w.CloseField(FieldObject)
	_, _ = w.CloseField(FieldObject)
	_, _ = w.CloseField(FieldArray)
	w.Close()

	result := buf.String()

	if result != expected {
		helperReportStringMismatch(t, expected, result)
	}
}

func TestJsonWriter_SingleNonEmptySubkey(t *testing.T) {
	expected := `{
  "data": [
    {
      "subkey": {
        "key": true
      }
    }
  ]
}`

	b := make([]byte, 0, 1024)
	buf := bytes.NewBuffer(b)
	w := NewJsonWriter(buf)
	_, _ = w.OpenField("data", FieldArray)
	_, _ = w.OpenField("", FieldObject)
	_, _ = w.OpenField("subkey", FieldObject)
	_, _ = w.Write([]byte(`"key": true`))
	_, _ = w.CloseField(FieldObject)
	_, _ = w.CloseField(FieldObject)
	_, _ = w.CloseField(FieldArray)
	w.Close()

	result := buf.String()

	if result != expected {
		helperReportStringMismatch(t, expected, result)
	}
}

func TestJsonWriter_SingleNonEmptySubkeyMultipleChildren(t *testing.T) {
	expected := `{
  "data": [
    {
      "subkey": {
        "key1": true,
        "key2": false
      }
    }
  ]
}`

	b := make([]byte, 0, 1024)
	buf := bytes.NewBuffer(b)
	w := NewJsonWriter(buf)
	_, _ = w.OpenField("data", FieldArray)
	_, _ = w.OpenField("", FieldObject)
	_, _ = w.OpenField("subkey", FieldObject)
	_, _ = w.Write([]byte(`"key1": true`))
	_, _ = w.Write([]byte(`"key2": false`))
	_, _ = w.CloseField(FieldObject)
	_, _ = w.CloseField(FieldObject)
	_, _ = w.CloseField(FieldArray)
	w.Close()

	result := buf.String()

	if result != expected {
		helperReportStringMismatch(t, expected, result)
	}
}

func TestJsonWriter_MultipleItems(t *testing.T) {
	expected := `{
  "first": 1,
  "second": 2
}`
	b := make([]byte, 0, 1024)
	buf := bytes.NewBuffer(b)
	w := NewJsonWriter(buf)
	_, err := w.Write([]byte(`"first": 1`))
	if err != nil {
		t.Fatal("first write:", err)
	}
	_, err = w.Write([]byte(`"second": 2`))
	if err != nil {
		t.Fatal("second write:", err)
	}
	w.Close()
	result := buf.String()

	if result != expected {
		helperReportStringMismatch(t, expected, result)
	}
}

func TestJsonWriter_MultipleItemsWithSubitems(t *testing.T) {
	expected := `{
  "data": [
    {
      "item": true
    }
  ],
  "errors": [],
  "meta": {}
}`
	b := make([]byte, 0, 1024)
	buf := bytes.NewBuffer(b)
	w := NewJsonWriter(buf)
	_, err := w.OpenField("data", FieldArray)
	if err != nil {
		t.Fatal(err)
	}
	_, err = w.OpenField("", FieldObject)
	if err != nil {
		t.Fatal(err)
	}
	_, err = w.Write([]byte(`"item": true`))
	if err != nil {
		t.Fatal(err)
	}
	_, _ = w.CloseField(FieldObject)
	_, _ = w.CloseField(FieldArray)

	_, _ = w.OpenField("errors", FieldArray)
	_, _ = w.CloseField(FieldArray)

	_, _ = w.OpenField("meta", FieldObject)
	_, _ = w.CloseField(FieldObject)

	w.Close()

	result := buf.String()

	if result != expected {
		helperReportStringMismatch(t, expected, result)
	}
}

func TestJsonWriter_openRoot(t *testing.T) {
	expected := `{`

	b := make([]byte, 0, 1024)
	buf := bytes.NewBuffer(b)
	w := NewJsonWriter(buf)
	_, _ = w.Write([]byte(""))

	result := buf.String()

	if result != expected {
		helperReportStringMismatch(t, expected, result)
	}
}

func TestJsonWriter_CloseApiResponse(t *testing.T) {
	expected := `{
  "data": [
    {
      "item": true
    }
  ],
  "meta": {
    "client": 0,
    "finalized": 0,
    "staging": 0,
    "ripe": 0,
    "unripe": 0
  },
  "errors": [
    "error1",
    "error2"
  ]
}`
	b := make([]byte, 0, 1024)
	buf := bytes.NewBuffer(b)
	w := NewJsonWriter(buf)
	w.GetMeta = func() (*types.MetaData, error) {
		return &types.MetaData{}, nil
	}
	w.ShouldWriteMeta = true

	_, err := w.OpenField("data", FieldArray)
	if err != nil {
		t.Fatal(err)
	}
	_, err = w.OpenField("", FieldObject)
	if err != nil {
		t.Fatal(err)
	}
	_, err = w.Write([]byte(`"item": true`))
	if err != nil {
		t.Fatal(err)
	}
	_, _ = w.CloseField(FieldObject)
	_, _ = w.CloseField(FieldArray)

	w.WriteError(errors.New("error1"))
	w.WriteError(errors.New("error2"))

	w.Close()

	result := buf.String()
	if result != expected {
		helperReportStringMismatch(t, expected, result)
	}
}

func TestJsonWriter_WriteCompound(t *testing.T) {
	expected := `{
  "data": [
    {
      "key": true
    }
  ]
}`

	b := make([]byte, 0, 1024)
	buf := bytes.NewBuffer(b)
	w := NewJsonWriter(buf)
	w.DefaultField = DefaultField{
		Key:       "data",
		FieldType: FieldArray,
	}
	_, _ = w.WriteCompoundItem("", map[string]bool{"key": true})
	w.Close()

	result := buf.String()
	if result != expected {
		helperReportStringMismatch(t, expected, result)
	}
}

func TestJsonWriter_WriteCompoundTwice(t *testing.T) {
	expected := `{
  "data": [
    {
      "first": true
    },
    {
      "second": true
    }
  ]
}`

	b := make([]byte, 0, 1024)
	buf := bytes.NewBuffer(b)
	w := NewJsonWriter(buf)
	w.DefaultField = DefaultField{
		Key:       "data",
		FieldType: FieldArray,
	}
	_, _ = w.WriteCompoundItem("", map[string]bool{"first": true})
	_, _ = w.WriteCompoundItem("", map[string]bool{"second": true})
	w.Close()

	result := buf.String()
	if result != expected {
		helperReportStringMismatch(t, expected, result)
	}
}

func TestJsonWriter_DefaultField(t *testing.T) {
	expected := `{
  "data": [
    {
      "subkey": {}
    }
  ]
}`

	b := make([]byte, 0, 1024)
	buf := bytes.NewBuffer(b)
	w := NewJsonWriter(buf)
	w.DefaultField = DefaultField{
		Key:       "data",
		FieldType: FieldArray,
	}
	_, _ = w.OpenField("", FieldObject)
	_, _ = w.OpenField("subkey", FieldObject)
	_, _ = w.CloseField(FieldObject)
	_, _ = w.CloseField(FieldObject)
	w.Close()

	result := buf.String()
	if result != expected {
		helperReportStringMismatch(t, expected, result)
	}
}

func TestJsonWriter_DefaultFieldEmpty(t *testing.T) {
	expected := `{
  "data": []
}`

	b := make([]byte, 0, 1024)
	buf := bytes.NewBuffer(b)
	w := NewJsonWriter(buf)
	w.DefaultField = DefaultField{
		Key:       "data",
		FieldType: FieldArray,
	}
	w.Close()

	result := buf.String()
	if result != expected {
		helperReportStringMismatch(t, expected, result)
	}
}

func TestJsonWriter_DefaultFieldCompound(t *testing.T) {
	expected := `{
  "data": [
    {}
  ]
}`

	b := make([]byte, 0, 1024)
	buf := bytes.NewBuffer(b)
	w := NewJsonWriter(buf)
	w.DefaultField = DefaultField{
		Key:       "data",
		FieldType: FieldArray,
	}
	_, _ = w.WriteCompoundItem("", map[string]string{})
	w.Close()

	result := buf.String()

	if result != expected {
		helperReportStringMismatch(t, expected, result)
	}
}
