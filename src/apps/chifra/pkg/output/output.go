// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package output

import (
	"bytes"
	"encoding/csv"
	"encoding/json"
	"fmt"
	"io"
	"reflect"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
)

// TODO: Fix export without arrays
func Output3(w io.Writer, data interface{}, format, chain string, hideHeader, testMode bool) error {
	nonEmptyFormat := format
	if format == "" || format == "none" {
		nonEmptyFormat = "json"
	}

	var outputBytes []byte
	var err error

	switch nonEmptyFormat {
	case "api":
		fallthrough
	case "json":
		var meta *rpcClient.MetaData = nil
		if format != "json" {
			meta = rpcClient.GetMetaData(chain, testMode)
		}
		result := JsonResult{Data: data, Meta: meta}
		outputBytes, err = json.MarshalIndent(result, "", "  ")
	case "csv":
		records, err := toStringRecords(data, true /* quote */, hideHeader)
		if err == nil {
			result := []string{}
			for _, row := range records {
				result = append(result, strings.Join(row, ","))
			}
			outputBytes = []byte(
				strings.Join(result, "\n"),
			)
		}
	case "txt":
		records, err := toStringRecords(data, false /* quote */, hideHeader)
		if err == nil {
			out := bytes.Buffer{}
			writer := csv.NewWriter(&out)
			writer.Comma = '\t'
			err = writer.WriteAll(records)
			if err == nil {
				outputBytes = out.Bytes()
			}
		}
	default:
		return fmt.Errorf("unsupported format %s", nonEmptyFormat)
	}

	if err != nil {
		return err
	}

	w.Write(outputBytes)
	return nil
}

// TODO: Fix export without arrays
func Output2(w io.Writer, data interface{}, format, chain string, hideHeader, testMode bool) error {
	nonEmptyFormat := format
	if format == "" || format == "none" {
		nonEmptyFormat = "txt"
	}
	return Output3(w, data, nonEmptyFormat, chain, hideHeader, testMode)
}

type JsonResult struct {
	Data interface{}         `json:"data,omitempty"`
	Meta *rpcClient.MetaData `json:"meta,omitempty"`
}

// toStringRecords uses Reflect API to read data from the provided slice of structs and
// turns it into a slice of string slices that can be later passed to encoding package
// writers to convert between different output formats
func toStringRecords(data interface{}, quote bool, hideHeader bool) ([][]string, error) {
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
