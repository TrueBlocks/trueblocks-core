package output

import (
	"strings"
	"testing"
)

func TestAsCsv(t *testing.T) {
	type TestType struct {
		First  string
		Second string
	}

	input := []TestType{
		{"first first", "first second"},
		{"second first", "second second"},
	}

	result, err := AsCsv(input)
	if err != nil {
		t.Error(err)
		return
	}
	expected := strings.Join([]string{
		`"first","second"`,
		`"first first","first second"`,
		`"second first","second second"`,
	}, "\n")

	if result != expected {
		t.Error("Wrong result", result)
	}
}
