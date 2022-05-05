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
	"net/http"
	"reflect"
	"strings"
	"text/tabwriter"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type JsonFormatted struct {
	Data   interface{}         `json:"data,omitempty"`
	Errors []string            `json:"errors,omitempty"`
	Meta   *rpcClient.MetaData `json:"meta,omitempty"`
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
		strings.Join(result, "\n") + "\n",
	), nil
}

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

// AsJsonBytes1 marshals JsonFormatted struct, populating Meta field if needed
func AsJsonBytes1(j *JsonFormatted, data interface{}, format, chain string, testMode bool) ([]byte, error) {
	var result JsonFormatted

	if format == "json" {
		if len(j.Errors) > 0 {
			result.Errors = j.Errors
		} else {
			result.Data = j.Data
		}
	} else {
		if len(j.Errors) > 0 {
			result.Errors = j.Errors
		} else {
			result.Data = j.Data
			result.Meta = rpcClient.GetMetaData(chain, testMode)
		}
	}

	marshalled, err := json.MarshalIndent(result, "", "  ")
	if err != nil {
		return nil, err
	}

	return marshalled, err
}

// JsonFormatter marshals JsonFormatted struct, populating Meta field if needed
func JsonFormatter(data interface{}, format, chain string, testMode bool) ([]byte, error) {
	j := &JsonFormatted{}
	err, ok := data.(error)
	if ok {
		j.Errors = []string{
			err.Error(),
		}
	} else {
		j.Data = data
	}

	var result JsonFormatted

	if format == "json" {
		if len(j.Errors) > 0 {
			result.Errors = j.Errors
		} else {
			result.Data = j.Data
		}
	} else {
		if len(j.Errors) > 0 {
			result.Errors = j.Errors
		} else {
			result.Data = j.Data
			result.Meta = rpcClient.GetMetaData(chain, testMode)
		}
	}

	marshalled, err := json.MarshalIndent(result, "", "  ")
	if err != nil {
		return nil, err
	}

	return marshalled, err
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

// TODO: Fix export without arrays
func Output2(where io.Writer, data interface{}, format, chain string, hideHeader, testMode bool) error {
	nonEmptyFormat := format
	if format == "" || format == "none" {
		if utils.IsApiMode() {
			nonEmptyFormat = "api"
		} else {
			nonEmptyFormat = "txt"
		}
	}

	var outputBytes []byte
	var err error

	switch nonEmptyFormat {
	case "api":
		fallthrough
	case "json":
		outputBytes, err = JsonFormatter(data, nonEmptyFormat, chain, testMode)
	case "csv":
		outputBytes, err = CsvFormatter(data, hideHeader)
	case "txt":
		_, ok := where.(http.ResponseWriter)
		if !utils.IsTerminal() || ok {
			outputBytes, err = TxtFormatter(data, hideHeader)
		} else {
			// Use a table only on the command line when we're not re-directed or piped
			outputBytes, err = TabFormatter(data, hideHeader)
		}
	default:
		err = fmt.Errorf("unsupported format %s", format)
	}
	if err != nil {
		return err
	}

	where.Write(outputBytes)
	return nil
}
