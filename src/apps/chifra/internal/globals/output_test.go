package globals

/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
import (
	"reflect"
	"strings"
	"testing"
)

func TestToStringRecords(t *testing.T) {
	type TestType struct {
		First  string
		Second string
	}

	input := []TestType{
		{"first first", "first second"},
		{"second first", "second second"},
	}

	result, err := ToStringRecords(input, false)
	if err != nil {
		t.Error(err)
		return
	}
	expected := [][]string{
		{"first", "second"},
		{"first first", "first second"},
		{"second first", "second second"},
	}

	if !reflect.DeepEqual(result, expected) {
		t.Error("Wrong result", result)
	}
}

func TestAsCsv(t *testing.T) {
	type TestType struct {
		First  string
		Second string
	}

	input := []TestType{
		{"first first", "first second"},
		{"second first", "second second"},
	}

	var opts GlobalOptionsType
	opts.Format = "csv"
	result, err := opts.CsvFormatter(input)
	if err != nil {
		t.Error(err)
		return
	}
	expected := strings.Join([]string{
		`"first","second"`,
		`"first first","first second"`,
		`"second first","second second"`,
	}, "\n") + "\n"

	resultStr := string(result)
	if string(resultStr) != expected {
		t.Error("Wrong result", resultStr)
	}
}
