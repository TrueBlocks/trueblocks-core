package output

import (
	"bytes"
	"fmt"
	"io"
	"net/http"
	"text/tabwriter"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// Formatter type represents a function that can be used to format data
type Formatter func(data interface{}) ([]byte, error)

// The map below maps format string to Formatter
var formatStringToFormatter = map[string]Formatter{
	"api":  JsonFormatter,
	"json": JsonFormatter,
	"csv":  AsCsv,
	"txt":  TxtFormatter,
	"tab":  TabFormatter,
}

// JsonFormatter turns data into JSON
func JsonFormatter(data interface{}) ([]byte, error) {
	formatted := &JsonFormatted{}
	err, ok := data.(error)
	if ok {
		formatted.Errors = []string{
			err.Error(),
		}
	} else {
		formatted.Data = data
	}

	return AsJsonBytes(formatted)
}

// TxtFormatter turns data into TSV string
func TxtFormatter(data interface{}) ([]byte, error) {
	out := bytes.Buffer{}
	tsv, err := AsTsv(data)
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
func TabFormatter(data interface{}) ([]byte, error) {
	tabOutput := &bytes.Buffer{}
	tab := tabwriter.NewWriter(tabOutput, 0, 0, 2, ' ', 0)
	tsv, err := AsTsv(data)
	if err != nil {
		return nil, err
	}
	tab.Write(tsv)
	err = tab.Flush()

	return tabOutput.Bytes(), err
}

// Output converts data into the given format and writes to where writer
func Output(where io.Writer, format string, data interface{}) error {
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
	formatter, ok := formatStringToFormatter[nonEmptyFormat]
	if !ok {
		return fmt.Errorf("unsupported format %s", format)
	}

	output, err := formatter(data)
	if err != nil {
		return err
	}

	where.Write(output)
	// Maintain newline compatibility with C++ version
	if nonEmptyFormat == "json" || nonEmptyFormat == "api" {
		where.Write([]byte{'\n'})
	}
	return nil
}
