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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// TODO: Fix export without arrays
func OutputHeader(data interface{}, w io.Writer, format string) error {
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
func OutputSlice(data interface{}, w io.Writer, format string, hideHeader, first bool, meta *rpcClient.MetaData) error {
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
		if strings.Contains(format, "\t") || strings.Contains(format, ",") {
			tt := reflect.TypeOf(data)
			rowTemplate, err := GetRowTemplate(&tt, format)
			if err != nil {
				return err
			}
			return rowTemplate.Execute(w, data)
		}
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
func OutputObject(data interface{}, w io.Writer, format string, hideHeader, first bool, meta *rpcClient.MetaData) error {
	var outputBytes []byte
	var err error

	preceeds := ""
	switch format {
	case "api":
		fallthrough
	case "json":
		outputBytes, err = json.MarshalIndent(data, "", "  ")
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
		if strings.Contains(format, "\t") || strings.Contains(format, ",") {
			tt := reflect.TypeOf(data)
			rowTemplate, err := GetRowTemplate(&tt, format)
			if err != nil {
				return err
			}
			return rowTemplate.Execute(w, data)
		}
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
func GetHeader(t *reflect.Type, format string) string {
	fields, sep, quote := utils.GetFields(t, format, true)
	var sb strings.Builder
	for i, field := range fields {
		if i > 0 {
			sb.WriteString(sep)
		}
		sb.WriteString(quote + field + quote)
	}
	return sb.String()
}

// TODO: Fix export without arrays
// TODO: this should be cached so it doesn't need to use reflection
func GetRowTemplate(t *reflect.Type, format string) (*template.Template, error) {
	fields, sep, quote := utils.GetFields(t, format, false)
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
