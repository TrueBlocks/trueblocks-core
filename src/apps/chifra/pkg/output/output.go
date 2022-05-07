// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package output

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"reflect"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type JsonFormatted struct {
	Data   interface{}         `json:"data,omitempty"`
	Errors []string            `json:"errors,omitempty"`
	Meta   *rpcClient.MetaData `json:"meta,omitempty"`
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
		outputBytes, err = jsonFormatter(data, nonEmptyFormat, chain, testMode)
	case "csv":
		outputBytes, err = csvFormatter(data, hideHeader)
	case "txt":
		_, ok := where.(http.ResponseWriter)
		if !utils.IsTerminal() || ok {
			outputBytes, err = txtFormatter(data, hideHeader)
		} else {
			// Use a table only on the command line when we're not re-directed or piped
			outputBytes, err = tabFormatter(data, hideHeader)
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
