// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package output

import (
	"bytes"
	"encoding/csv"
	"encoding/json"
	"strings"
	"text/tabwriter"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
)

// jsonFormatter marshals JsonFormatted struct, populating Meta field if needed
func jsonFormatter(data interface{}, format, chain string, testMode bool) ([]byte, error) {
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

// csvFormatter turns a type into CSV string. It uses custom code instead of
// Go's encoding/csv to maintain compatibility with C++ output, which
// quotes each item. encoding/csv would double-quote a quoted string...
func csvFormatter(i interface{}, hideHeader bool) ([]byte, error) {
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

// txtFormatter turns data into TSV string
func txtFormatter(data interface{}, hideHeader bool) ([]byte, error) {
	out := bytes.Buffer{}
	tsv, err := asTsv(data, hideHeader)
	if err != nil {
		return nil, err
	}
	_, err = out.Write(tsv)
	if err != nil {
		return nil, err
	}
	return out.Bytes(), nil
}

// tabFormatter turns data into a table (string)
func tabFormatter(data interface{}, hideHeader bool) ([]byte, error) {
	tabOutput := &bytes.Buffer{}
	tab := tabwriter.NewWriter(tabOutput, 0, 0, 2, ' ', 0)
	tsv, err := asTsv(data, hideHeader)
	if err != nil {
		return nil, err
	}
	tab.Write(tsv)
	err = tab.Flush()

	return tabOutput.Bytes(), err
}

// asTsv turns a type into tab-separated values
func asTsv(data interface{}, hideHeader bool) ([]byte, error) {
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
