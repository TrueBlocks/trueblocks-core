// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package output

import (
	"bytes"
	"encoding/csv"
	"fmt"
	"reflect"
	"strings"
	"text/tabwriter"
)

// TxtFormatter turns data into TSV string
func TxtFormatter(data interface{}, hideHeader bool) ([]byte, error) {
	out := bytes.Buffer{}
	tsv, err := AsTsv(data, hideHeader)
	if err != nil {
		return nil, err
	}
	_, err = out.Write(tsv)
	if err != nil {
		return nil, err
	}
	return out.Bytes(), nil
}

// TabFormatter turns data into a table (string)
func TabFormatter(data interface{}, hideHeader bool) ([]byte, error) {
	tabOutput := &bytes.Buffer{}
	tab := tabwriter.NewWriter(tabOutput, 0, 0, 2, ' ', 0)
	tsv, err := AsTsv(data, hideHeader)
	if err != nil {
		return nil, err
	}
	tab.Write(tsv)
	err = tab.Flush()

	return tabOutput.Bytes(), err
}

// CsvFormatter turns a type into CSV string. It uses custom code instead of
// Go's encoding/csv to maintain compatibility with C++ output, which
// quotes each item. encoding/csv would double-quote a quoted string...
func CsvFormatter(i interface{}, hideHeader bool) ([]byte, error) {
	records, err := ToStringRecords(i, true, hideHeader)
	if err != nil {
		return nil, err
	}
	result := []string{}
	// We have to join records in one row with a ","
	for _, row := range records {
		result = append(result, strings.Join(row, ","))
	}

	// Now we need to join all rows with a newline and add an ending newline
	// top match the .txt output
	return []byte(
		strings.Join(result, "\n"),
	), nil
}

// AsTsv turns a type into tab-separated values
func AsTsv(data interface{}, hideHeader bool) ([]byte, error) {
	records, err := ToStringRecords(data, false, hideHeader)
	if err != nil {
		return nil, err
	}

	buf := &bytes.Buffer{}
	writer := csv.NewWriter(buf)
	writer.Comma = '\t'

	err = writer.WriteAll(records)
	if err != nil {
		return nil, err
	}

	err = writer.Error()
	if err != nil {
		return nil, err
	}

	return buf.Bytes(), nil
}

// ToStringRecords uses Reflect API to read data from the provided slice of structs and
// turns it into a slice of string slices that can be later passed to encoding package
// writers to convert between different output formats
func ToStringRecords(data interface{}, quote bool, hideHeader bool) ([][]string, error) {
	var records [][]string
	// We can quote the data now, so that we don't have to loop over it again
	// later.
	format := "%v"
	if quote {
		format = `"%v"`
	}
	header := []string{}
	// We only support slice of structs
	slice := reflect.ValueOf(data)
	if slice.Kind() != reflect.Slice {
		return nil, fmt.Errorf("%s is not a structure", slice.Type().Name())
	}
	insideType := reflect.TypeOf(slice.Index(0).Interface())
	if insideType.Kind() != reflect.Struct {
		return nil, fmt.Errorf("%s is not a struct", insideType.Name())
	}

	for i := 0; i < slice.Len(); i++ {
		var record []string
		// Read the struct
		content := slice.Index(i).Interface()
		strct := reflect.ValueOf(content)

		for j := 0; j < strct.NumField(); j++ {
			// Now read each field from it and put into record
			field := strct.Field(j).Interface()
			record = append(record, fmt.Sprintf(format, field))

			// If it's our first iteration, we save the struct's key names
			// to use them as headers
			if i == 0 {
				header = append(header, fmt.Sprintf(format, makeFirstLowerCase(insideType.Field(j).Name)))
			}
		}
		records = append(records, record)
	}

	var result [][]string
	if !hideHeader {
		result = append(result, header)
	}
	result = append(result, records...)
	return result, nil
}

func makeFirstLowerCase(s string) string {
	if len(s) < 2 {
		return strings.ToLower(s)
	}
	bts := []byte(s)
	lc := bytes.ToLower([]byte{bts[0]})
	rest := bts[1:]
	return string(bytes.Join([][]byte{lc, rest}, nil))
}
