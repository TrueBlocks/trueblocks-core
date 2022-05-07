// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package output

// import (
// 	"reflect"
// 	"strings"
// 	"testing"
// )

// func TestToStringRecords(t *testing.T) {
// 	type TestType struct {
// 		First  string
// 		Second string
// 	}

// 	input := []TestType{
// 		{"first first", "first second"},
// 		{"second first", "second second"},
// 	}

// 	result, err := ToStringRecords(input, false, false)
// 	if err != nil {
// 		t.Error(err)
// 		return
// 	}
// 	expected := [][]string{
// 		{"first", "second"},
// 		{"first first", "first second"},
// 		{"second first", "second second"},
// 	}

// 	if !reflect.DeepEqual(result, expected) {
// 		t.Error("Wrong result", result)
// 	}
// }

// func TestAsCsv(t *testing.T) {
// 	type TestType struct {
// 		First  string
// 		Second string
// 	}

// 	input := []TestType{
// 		{"first first", "first second"},
// 		{"second first", "second second"},
// 	}

// 	var opts GlobalOptions
// 	opts.Format = "csv"
// 	result, err := opts.CsvFormatter(input, false)
// 	if err != nil {
// 		t.Error(err)
// 		return
// 	}
// 	expected := strings.Join([]string{
// 		`"first","second"`,
// 		`"first first","first second"`,
// 		`"second first","second second"`,
// 	}, "\n") + "\n"

// 	resultStr := string(result)
// 	if string(resultStr) != expected {
// 		t.Error("Wrong result", resultStr)
// 	}
// }
