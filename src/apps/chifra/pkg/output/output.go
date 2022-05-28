// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package output

import (
	"encoding/json"
	"fmt"
	"html/template"
	"io"
	"reflect"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func OutputHeader(data interface{}, w io.Writer, format string, apiMode bool) error {
	if format == "txt" || format == "csv" {
		tt := reflect.TypeOf(data)
		w.Write([]byte(GetHeader(&tt, format)))
		w.Write([]byte("\n"))
	} else {
		w.Write([]byte("{\n  \"data\": [\n    "))
	}
	return nil
}

// TODO: Fix export without arrays
func OutputSlice(data interface{}, w io.Writer, format string, hideHeader, apiMode, first bool, meta *rpcClient.MetaData) error {
	var outputBytes []byte
	var err error

	preceeds := ""
	switch format {
	case "api":
		fallthrough
	case "json":
		result := struct {
			Data interface{}         `json:"data,omitempty"`
			Meta *rpcClient.MetaData `json:"meta,omitempty"`
		}{Data: data, Meta: meta}
		outputBytes, err = json.MarshalIndent(result, "", "  ")
		if !first {
			preceeds = ","
		}
	case "csv":
		fallthrough
	case "txt":
		tt := reflect.TypeOf(data)
		rowTemplate, err := GetRowTemplate(&tt, format)
		if err != nil {
			return err
		}
		return rowTemplate.Execute(w, data)
	default:
		return fmt.Errorf("unsupported format %s", format)
	}

	if err != nil {
		return err
	}
	w.Write([]byte(preceeds))
	w.Write(outputBytes)

	return nil
}

// TODO: Fix export without arrays
func OutputObject(data interface{}, w io.Writer, format string, hideHeader, apiMode, first bool, meta *rpcClient.MetaData) error {
	var outputBytes []byte
	var err error

	preceeds := ""
	switch format {
	case "api":
		fallthrough
	case "json":
		outputBytes, err = json.MarshalIndent(data, "    ", "  ")
		if err != nil {
			return err
		}
		if !first {
			preceeds = ","
		}
	case "csv":
		fallthrough
	case "txt":
		tt := reflect.TypeOf(data)
		rowTemplate, err := GetRowTemplate(&tt, format)
		if err != nil {
			return err
		}
		return rowTemplate.Execute(w, data)
	default:
		return fmt.Errorf("unsupported format %s", format)
	}

	if err != nil {
		return err
	}
	w.Write([]byte(preceeds))
	w.Write(outputBytes)

	return nil
}

func getFields(t *reflect.Type, format string, header bool) (fields []string, sep string, quote string) {
	if (*t).Kind() != reflect.Struct {
		logger.Fatal((*t).Name() + " is not a structure")
	}
	for i := 0; i < (*t).NumField(); i++ {
		fn := (*t).Field(i).Name
		if header {
			fields = append(fields, utils.MakeFirstLowerCase(fn))
		} else {
			fields = append(fields, fn)
		}
	}
	sep = "\t"
	quote = ""
	if format == "csv" {
		sep = ","
		quote = "\""
	}
	return fields, sep, quote
}

func GetHeader(t *reflect.Type, format string) string {
	fields, sep, quote := getFields(t, format, true)
	var sb strings.Builder
	for i, field := range fields {
		if i > 0 {
			sb.WriteString(sep)
		}
		sb.WriteString(quote + field + quote)
	}
	return sb.String()
}

func GetRowTemplate(t *reflect.Type, format string) (*template.Template, error) {
	fields, sep, quote := getFields(t, format, false)
	var sb strings.Builder
	for i, field := range fields {
		if i > 0 {
			sb.WriteString(sep)
		}
		sb.WriteString(quote + "{{." + field + "}}" + quote)
	}
	tt, err := template.New("").Parse(sb.String() + "\n")
	return tt, err
}
