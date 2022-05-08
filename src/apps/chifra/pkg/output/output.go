// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package output

import (
	"encoding/json"
	"fmt"
	"io"
	"reflect"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
)

// TODO: Fix export without arrays
func OutputArray(data []interface{}, w io.Writer, format string, hideHeader, apiMode bool, meta *rpcClient.MetaData) error {
	switch format {
	case "api":
		fallthrough
	case "json":
		return OutputObject(data, w, format, hideHeader, apiMode, meta)
	case "csv":
		fallthrough
	case "txt":
		for i, item := range data {
			if i == 0 {
				if !hideHeader {
					err := OutputHeader(item, w, format, apiMode)
					if err != nil {
						return err
					}
				}
			}
			err := OutputObject(item, w, format, hideHeader, apiMode, meta)
			if err != nil {
				return err
			}
		}
		return nil
	}
	return fmt.Errorf("unsupported format %s", format)
}

// TODO: Fix export without arrays
func OutputObject(data interface{}, w io.Writer, format string, hideHeader, apiMode bool, meta *rpcClient.MetaData) error {
	var outputBytes []byte
	var err error

	switch format {
	case "api":
		fallthrough
	case "json":
		result := struct {
			Data interface{}         `json:"data,omitempty"`
			Meta *rpcClient.MetaData `json:"meta,omitempty"`
		}{Data: data, Meta: meta}
		outputBytes, err = json.MarshalIndent(result, "", "  ")
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
	w.Write(outputBytes)

	return nil
}

func OutputHeader(data interface{}, w io.Writer, format string, apiMode bool) error {
	if format == "txt" || format == "csv" {
		tt := reflect.TypeOf(data)
		w.Write([]byte(GetHeader(&tt, format)))
		w.Write([]byte("\n"))
	}
	return nil
}
