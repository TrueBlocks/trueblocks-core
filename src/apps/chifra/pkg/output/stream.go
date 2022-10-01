package output

import (
	"context"
	"encoding/csv"
	"encoding/json"
	"fmt"
	"io"
	"strings"
	"sync"
	"text/template"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// OutputOptions allow more granular configuration of output details
// TODO (dawid): This used to be "type OutputOptions = struct" (the '=' sign). Was that a typo or purposful? I couldn't embed it in the GlobalOptions data structure, so I removed the '='
type OutputOptions struct {
	// If set, raw data from the RPC will be printed instead of the model
	ShowRaw bool
	// If set, hidden fields will be printed as well (depends on the format)
	Verbose bool
	// If Verbose is true, this is the level of detail (verbose alone implies LogLevel=1)
	LogLevel uint64
	// If set, the first line of "txt" and "csv" output will NOT (the keys) will squelched
	NoHeader bool
	// The format in which to print the output
	Format string
	// How to indent JSON output
	JsonIndent string
	// Chain name
	Chain string
	// Flag to check if we are in test mode
	TestMode bool
	// Output file name. If present, we will write output to this file
	OutputFn string
	// If true and OutputFn is non-empty, open OutputFn for appending (create if not present)
	Append bool
	// The writer
	Writer io.Writer
}

var formatToSeparator = map[string]rune{
	"csv": ',',
	"txt": '\t',
}

// StreamWithTemplate executes a template `tmpl` over Model `model`
func StreamWithTemplate(w io.Writer, model types.Model, tmpl *template.Template) error {
	return tmpl.Execute(w, model.Data)
}

// StreamModel streams a single `Model`
func StreamModel(w io.Writer, model types.Model, options OutputOptions) error {
	if options.Format == "json" || options.Format == "api" {
		v, err := json.MarshalIndent(model.Data, "    ", options.JsonIndent)
		if err != nil {
			return err
		}
		w.Write(v)
		return nil
	}

	// Store map items as strings. All formats other than JSON need string data
	strs := make([]string, 0, len(model.Order))
	for _, key := range model.Order {
		strs = append(strs, fmt.Sprint(model.Data[key]))
	}

	var separator rune
	if len(options.Format) == 1 {
		separator = rune(options.Format[0])
	} else {
		separator = formatToSeparator[options.Format]
	}
	if separator == 0 {
		return fmt.Errorf("unknown format %s", options.Format)
	}
	outputWriter := csv.NewWriter(w)
	outputWriter.Comma = rune(separator)
	if !options.NoHeader { // notice double negative
		outputWriter.Write(model.Order)
	}
	outputWriter.Write(strs)
	// This Flushes for each printed item, but in the exchange the user gets
	// the data printed as it comes
	outputWriter.Flush()

	err := outputWriter.Error()
	if err != nil {
		return err
	}

	return nil
}

// StreamRaw outputs raw `Raw` to `w`
func StreamRaw[Raw types.RawData](w io.Writer, raw *Raw) (err error) {
	bytes, err := json.MarshalIndent(raw, "", "  ")
	if err != nil {
		return
	}
	w.Write(bytes)
	// Add a newline so that the command prompt is not being printed at
	// the same line as the output
	w.Write([]byte("\n"))
	return
}

func writeJsonErrors(w io.Writer, errs []string, options *OutputOptions) error {
	marshalled, err := json.MarshalIndent(errs, "  ", options.JsonIndent)
	if err != nil {
		return err
	}
	w.Write(marshalled)
	return nil
}

func closeApiOrRawResponse(w io.Writer, errsToReport []string, options *OutputOptions) {
	if options.ShowRaw {
		// Raw data already includes a newline
		w.Write([]byte("  ]"))
	} else {
		w.Write([]byte("\n  ]"))
	}
	if options.Format == "api" {
		w.Write([]byte(",\n  \"meta\": "))
		meta := getMetaData(options.Chain, options.TestMode)
		b, _ := json.MarshalIndent(meta, "  ", options.JsonIndent)
		w.Write(b)
	}
	if options.Format == "api" && len(errsToReport) > 0 {
		// For API, we want to report errors under `errors` key in the response,
		// but only for "api" format...
		w.Write([]byte(",\n  \"errors\": "))
		err := writeJsonErrors(w, errsToReport, options)
		if err != nil {
			panic(err)
		}
	}
	w.Write([]byte("\n}\n"))
	if options.ShowRaw && options.Format != "api" {
		// ...if streaming other format, we want to print the errors to stderr
		printErrors(errsToReport)
	}
}

func printErrors(errsToReport []string) {
	for _, errMessage := range errsToReport {
		logger.Log(logger.Error, errMessage)
	}
}

func getMetaData(chain string, testMode bool) (meta *rpcClient.MetaData) {
	meta, err := rpcClient.GetMetaData(chain, testMode)
	if err != nil {
		// In case of error, we return empty MetaData to not break JSON
		return
	}
	return
}

// StreamMany outputs models or raw data as they are acquired
func StreamMany[Raw types.RawData](
	ctx context.Context,
	fetchData func(modelChan chan types.Modeler[Raw], errorChan chan error),
	options OutputOptions,
) error {
	outputWriter := options.GetOutputFileWriter()
	errsToReport := make([]string, 0)
	errsMutex := sync.Mutex{}

	modelChan := make(chan types.Modeler[Raw])
	errorChan := make(chan error)

	// Check if the current item is the first that we print. If so, we may want to
	// print keys or postpone adding JSON comma between the elements
	first := true
	// Start getting the data
	go func() {
		fetchData(modelChan, errorChan)
		close(modelChan)
		close(errorChan)
	}()

	// If we are printing JSON, we want to make sure that opening and closing
	// brackets are printed
	if options.Format == "json" {
		outputWriter.Write([]byte("{\n  \"data\": [\n    "))
		defer func() {
			outputWriter.Write([]byte("\n  ]\n}\n"))
			printErrors(errsToReport)
		}()
	}
	// If printing API format, we want to add meta information and errors
	if options.ShowRaw || options.Format == "api" {
		outputWriter.Write([]byte("{\n  \"data\": [\n    "))
		defer func() {
			closeApiOrRawResponse(outputWriter, errsToReport, &options)
		}()
	}

	defer func() {
		// We generally print errors for non-api formats, unless we are printing raw.
		// But printing errors for JSON has to be handled in`closeApiOrRawResponse`,
		// as we don't want to print errors before the closing `}`
		if options.Format == "json" || options.Format == "api" || options.ShowRaw {
			return
		}
		printErrors(errsToReport)
	}()

	// If user wants custom format, we have to prepare the template
	customFormat := strings.Contains(options.Format, "{")
	tmpl, err := template.New("").Parse(options.Format)
	if customFormat && err != nil {
		return err
	}

	for {
		select {
		case model, ok := <-modelChan:
			if !ok {
				return nil
			}

			// If the output is JSON and we are printing another item, put `,` in front of it
			if !first && (options.Format == "json" || options.Format == "api") {
				outputWriter.Write([]byte(","))
			}
			var err error
			if options.ShowRaw {
				err = StreamRaw(outputWriter, model.Raw())
			} else {
				modelValue := model.Model(options.Verbose, options.Format)
				if customFormat {
					err = StreamWithTemplate(outputWriter, modelValue, tmpl)
				} else {
					err = StreamModel(outputWriter, modelValue, OutputOptions{
						NoHeader:   !first || options.NoHeader,
						Format:     options.Format,
						JsonIndent: "  ",
					})
				}
			}
			if err != nil {
				return err
			}
			first = false

		case err, ok := <-errorChan:
			if !ok {
				continue
			}
			errsMutex.Lock()
			errsToReport = append(errsToReport, err.Error())
			errsMutex.Unlock()

		case <-ctx.Done():
			return ctx.Err()
		}
	}
}
