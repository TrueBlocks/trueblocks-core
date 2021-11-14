package globals

import (
	"bytes"
	"context"
	"encoding/csv"
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"os"
	"reflect"
	"strings"
	"text/tabwriter"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// Output converts data into the given format and writes to where writer
func Output(opts *GlobalOptionsType, where io.Writer, format string, data interface{}) error {
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

	var output []byte
	var err error

	switch nonEmptyFormat {
	case "api":
		fallthrough
	case "json":
		output, err = opts.JsonFormatter(data)
	case "csv":
		output, err = opts.CsvFormatter(data)
	case "txt":
		output, err = opts.TxtFormatter(data)
	case "tab":
		output, err = opts.TabFormatter(data)
	default:
		return fmt.Errorf("unsupported format %s", format)
	}

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

// JsonFormatter turns data into JSON
func (opts *GlobalOptionsType) JsonFormatter(data interface{}) ([]byte, error) {
	formatted := &JsonFormatted{}
	err, ok := data.(error)
	if ok {
		formatted.Errors = []string{
			err.Error(),
		}
	} else {
		formatted.Data = data
	}

	return AsJsonBytes(formatted, opts)
}

// TxtFormatter turns data into TSV string
func (opts *GlobalOptionsType) TxtFormatter(data interface{}) ([]byte, error) {
	out := bytes.Buffer{}
	tsv, err := opts.AsTsv(data)
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
func (opts *GlobalOptionsType) TabFormatter(data interface{}) ([]byte, error) {
	tabOutput := &bytes.Buffer{}
	tab := tabwriter.NewWriter(tabOutput, 0, 0, 2, ' ', 0)
	tsv, err := opts.AsTsv(data)
	if err != nil {
		return nil, err
	}
	tab.Write(tsv)
	err = tab.Flush()

	return tabOutput.Bytes(), err
}

// This type is used to carry CSV layout information
type CsvFormatted struct {
	Header  []string
	Content [][]string
}

// CsvFormatter turns a type into CSV string. It uses custom code instead of
// Go's encoding/csv to maintain compatibility with C++ output, which
// quotes each item. encoding/csv would double-quote a quoted string...
func (opts *GlobalOptionsType) CsvFormatter(i interface{}) ([]byte, error) {
	records, err := ToStringRecords(i, true)
	if err != nil {
		return nil, err
	}
	result := []string{}
	// We have to join records in one row with a ","
	for _, row := range records {
		result = append(result, strings.Join(row, ","))
	}

	// Now we need to join all rows with a newline. We also add one
	// final newline to be concise with both Go's encoding/csv and C++
	// version
	return []byte(
		strings.Join(result, "\n") + "\n",
	), nil
}

type JsonFormatted struct {
	Data   interface{} `json:"data,omitempty"`
	Errors []string    `json:"errors,omitempty"`
	Meta   *Meta       `json:"meta,omitempty"`
}

// AsJsonBytes marshals JsonFormatted struct, populating Meta field if
// needed
func AsJsonBytes(j *JsonFormatted, opts *GlobalOptionsType) ([]byte, error) {
	var result JsonFormatted

	if opts.Format == "json" {
		if len(validate.Errors) > 0 {
			result.Errors = validate.Errors
		} else {
			if len(j.Errors) > 0 {
				result.Errors = j.Errors
			} else {
				result.Data = j.Data
			}
		}
	} else {
		if len(j.Errors) > 0 {
			result.Errors = j.Errors
		} else {
			result.Data = j.Data
			result.Meta = GetMeta(opts.TestMode)
		}
	}

	marshalled, err := json.MarshalIndent(result, "", "  ")
	if err != nil {
		return nil, err
	}

	return marshalled, err
}

// PrintJson marshals its arguments and prints JSON in a standardized format
func PrintJson(j *JsonFormatted, opts *GlobalOptionsType) error {
	marshalled, err := AsJsonBytes(j, opts)
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

// AsTsv turns a type into tab-separated values
func (opts *GlobalOptionsType) AsTsv(data interface{}) ([]byte, error) {
	records, err := ToStringRecords(data, false)
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

type Meta struct {
	Unripe    string `json:"unripe"`
	Ripe      string `json:"ripe"`
	Staging   string `json:"staging"`
	Finalized string `json:"finalized"`
	Client    string `json:"client"`
}

func (m Meta) String() string {
	ret, _ := json.MarshalIndent(m, "", "  ")
	return string(ret)
}

func GetMeta(testMode bool) *Meta {
	if testMode {
		return &Meta{
			Unripe:    "0xdeadbeef",
			Ripe:      "0xdeadbeef",
			Staging:   "0xdeadbeef",
			Finalized: "0xdeadbeef",
			Client:    "0xdeadbeef",
		}
	}
	client := rpcClient.Get()
	bn, _ := client.BlockNumber(context.Background())
	return &Meta{Client: fmt.Sprintf("%d", bn)}
}

func MakeFirstLowerCase(s string) string {
	if len(s) < 2 {
		return strings.ToLower(s)
	}
	bts := []byte(s)
	lc := bytes.ToLower([]byte{bts[0]})
	rest := bts[1:]
	return string(bytes.Join([][]byte{lc, rest}, nil))
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

// ToStringRecords uses Reflect API to read data from the provided slice of structs and
// turns it into a slice of string slices that can be later passed to encoding package
// writers to convert between different output formats
func ToStringRecords(data interface{}, quote bool) ([][]string, error) {
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
				header = append(header, fmt.Sprintf(format, MakeFirstLowerCase(insideType.Field(j).Name)))
			}
		}
		records = append(records, record)
	}

	result := [][]string{
		header,
	}
	result = append(result, records...)
	return result, nil
}
