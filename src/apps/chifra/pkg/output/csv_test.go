package output

import (
	"fmt"
	"log"
	"strings"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/globals"
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

	var opts globals.GlobalOptionsType
	opts.Format = "csv"
	result, err := CsvFormatter(input, &opts)
	if err != nil {
		t.Error(err)
		return
	}
	str := strings.Trim(string(result), "\n")
	expected := strings.Join([]string{
		`"first","second"`,
		`"first first","first second"`,
		`"second first","second second"`,
	}, "\n")
	log.Println(str)
	log.Println(expected)

	if str != expected {
		fmt.Printf("*** %s ****\n", strings.Replace(str, "\n", "AAAA", -1))
		str := strings.Replace(string(result), expected, "++++", -1)
		t.Error("Wrong result: ", str, ":")
	}
}
