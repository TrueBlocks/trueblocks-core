// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package globals

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"os"
	"strings"
	"text/tabwriter"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// This type is used to carry CSV layout information
type CsvFormatted struct {
	Header  []string
	Content [][]string
}

type JsonFormatted struct {
	Data   interface{}         `json:"data,omitempty"`
	Errors []string            `json:"errors,omitempty"`
	Meta   *rpcClient.MetaData `json:"meta,omitempty"`
}

// AsJsonBytes marshals JsonFormatted struct, populating Meta field if needed
func AsJsonBytes(j *JsonFormatted, format, chain string, testMode bool) ([]byte, error) {
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

// PrintJson marshals its arguments and prints JSON in a standardized format
func PrintJson(j *JsonFormatted, opts *GlobalOptions) error {
	marshalled, err := AsJsonBytes(j, opts.Format, opts.Chain, opts.TestMode)
	if err != nil {
		return err
	}
	fmt.Fprintln(os.Stdout, string(marshalled))

	return nil
}

// Table makes it easier to output tabular data to the console
type Table struct {
	Writer *tabwriter.Writer
	target *os.File
}

// New sets up the default writer and target for a table
func (t *Table) New() {
	t.Writer = tabwriter.NewWriter(os.Stdout, 0, 0, 2, ' ', 0)
	t.target = os.Stdout
}

// Header prints table header to the writer
func (t *Table) Header(header []string) {
	fmt.Fprintln(t.Writer, strings.Join(header, "\t"))
}

// Row prints data as table cells in one row
func (t *Table) Row(cells []string) {
	for _, cell := range cells {
		fmt.Fprint(t.Writer, cell, "\t")
	}
	fmt.Fprint(t.Writer, "\n")
}

// Print flushes the Writer, which will print the table
func (t *Table) Print() error {
	return t.Writer.Flush()
}

func MakeFirstUpperCase(s string) string {
	if len(s) < 2 {
		return strings.ToLower(s)
	}
	bts := []byte(s)
	lc := bytes.ToUpper([]byte{bts[0]})
	rest := bts[1:]
	return string(bytes.Join([][]byte{lc, rest}, nil))
}

// TODO: Can we use golang templates for speed?
// func getFields(t *reflect.Type) (fields []string, sep string, quote string) {
// 	for i := 0; i < (*t).NumField(); i++ {
// 		fields = append(fields, MakeFirstLowerCase((*t).Field(i).Name))
// 	}
// 	sep = "\t"
// 	quote = ""
// 	if Format == "csv" {
// 		sep = ","
// 		quote = "\""
// 	}
// 	return fields, sep, quote
// }

// func GetHeader(t *reflect.Type) string {
// 	if (*t).Kind() != reflect.Struct {
// 		logger.Fatal((*t).Name() + " is not a structure")
// 	}

// 	fields, sep, quote := getFields(t)
// 	var sb strings.Builder
// 	for i, field := range fields {
// 		if i > 0 {
// 			sb.WriteString(sep)
// 		}
// 		sb.WriteString(quote + field + quote)
// 	}
// 	return sb.String()
// }

// func GetRowTemplate(t *reflect.Type) (*template.Template, error) {
// 	fields, sep, quote := getFields(t)
// 	var sb strings.Builder
// 	for i, field := range fields {
// 		if i > 0 {
// 			sb.WriteString(sep)
// 		}
// 		sb.WriteString(quote + "{{." + MakeFirstUpperCase(field) + "}}" + quote)
// 	}
// 	// fmt.Println(sb.String() + "\n")
// 	tt, err := template.New("").Parse(sb.String() + "\n")
// 	return tt, err
// }

var formatToMimeType = map[string]string{
	"api":  "application/json",
	"json": "application/json",
	"csv":  "text/csv",
	"txt":  "text/tab-separated-values",
}

// RespondWithError marshals the given error err into JSON
// that can be returned to the client and sets httpStatus HTTP error status code
func (opts *GlobalOptions) RespondWithError(w http.ResponseWriter, httpStatus int, err error) {
	marshalled, err := AsJsonBytes(&JsonFormatted{
		Errors: []string{err.Error()},
	}, opts.Format, opts.Chain, opts.TestMode)
	if err != nil {
		panic(err)
	}
	w.WriteHeader(httpStatus)
	w.Write(marshalled)
}

// TODO: Fix export without arrays
func (opts *GlobalOptions) OutputArray(data interface{}) error {
	formatNotEmpty := opts.Format
	if formatNotEmpty == "" {
		formatNotEmpty = "api"
	}

	if opts.ApiMode && opts.Writer != nil {
		// Decides which format should be used, calls the right Responder, sets HTTP
		// status code and writes a response
		opts.Writer.Header().Set("Content-Type", formatToMimeType[formatNotEmpty])
		opts.Format = formatNotEmpty
		err := opts.Output2(opts.Writer, data, opts.Format, opts.NoHeader)
		if err != nil {
			opts.RespondWithError(opts.Writer, http.StatusInternalServerError, err)
		}

	} else {
		err := opts.Output2(os.Stdout, data, opts.Format, opts.NoHeader)
		if err != nil {
			logger.Log(logger.Error, err)
		}
	}

	return nil
}

// TODO: Fix export without arrays
func (opts *GlobalOptions) Output2(where io.Writer, data interface{}, format string, hideHeader bool) error {
	nonEmptyFormat := format
	if format == "" || format == "none" {
		if utils.IsApiMode() {
			nonEmptyFormat = "api"
		} else {
			nonEmptyFormat = "txt"
		}
	}
	if nonEmptyFormat == "txt" {
		_, ok := where.(http.ResponseWriter)
		// We would never want to use tab format in server environment
		if utils.IsTerminal() && !ok {
			nonEmptyFormat = "tab"
		}
	}

	var outputBytes []byte
	var err error

	switch nonEmptyFormat {
	case "api":
		fallthrough
	case "json":
		formatted := &JsonFormatted{}
		err, ok := data.(error)
		if ok {
			formatted.Errors = []string{
				err.Error(),
			}
		} else {
			formatted.Data = data
		}
		outputBytes, err = AsJsonBytes(formatted, opts.Format, opts.Chain, opts.TestMode)
	case "csv":
		outputBytes, err = output.CsvFormatter(data, hideHeader)
	case "txt":
		outputBytes, err = output.TxtFormatter(data, hideHeader)
	case "tab":
		// TODO: There is no such case -- this is 'txt' in non-API mode with a terminal
		outputBytes, err = output.TabFormatter(data, hideHeader)
	default:
		return fmt.Errorf("unsupported format %s", format)
	}

	if err != nil {
		return err
	}

	where.Write(outputBytes)
	// Maintain newline compatibility with C++ version
	if nonEmptyFormat == "json" || nonEmptyFormat == "api" {
		where.Write([]byte{'\n'})
	}

	return nil
}
