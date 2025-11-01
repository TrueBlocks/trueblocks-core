package outputHelpers

import (
	"io"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
	"github.com/spf13/cobra"
)

var discardWriter = io.Writer(nil)

func init() {
	if os.Getenv("TB_DISCARD_WRITER") == "true" {
		discardWriter = io.Discard
	}
}

func PreRunWithJsonWriter(cmdName string, getOptions func() *globals.GlobalOptions) func(cmd *cobra.Command, args []string) {
	_ = cmdName
	return func(cmd *cobra.Command, args []string) {
		_ = cmd
		_ = args
		opts := getOptions()
		var outputWriter io.Writer
		// Prepare the default writer (stdout or --output file)
		outputWriter = os.Stdout
		if opts.OutputFn != "" {
			outputWriter = opts.GetOutputFileWriter()
		} else if discardWriter != nil {
			outputWriter = discardWriter
		}

		// If we need to output JSON, init JsonWriter...
		if opts.Format == "json" {
			jw := output.NewDefaultJsonWriter(outputWriter, true)
			opts.Writer = jw
		} else {
			// ...or set the default writer as global writer for the current command
			// invocation
			opts.Writer = outputWriter
		}
	}
}

func PostRunWithJsonWriter(getOptions func() *globals.GlobalOptions) func(cmd *cobra.Command, args []string) {
	return func(cmd *cobra.Command, args []string) {
		_ = cmd
		_ = args
		opts := getOptions()
		w := opts.Writer
		// Try to cast the global writer to JsonWriter
		jw, ok := w.(*output.JsonWriter)
		if !ok {
			// If it's not JsonWriter, do nothing
			return
		}
		// If it is JsonWriter, close it (print closing brackets)
		jw.Close()
	}
}

// SetWriterForCommand sets the writer for currently running command, but only if
// we are running with --file
func SetWriterForCommand(cmdName string, opts *globals.GlobalOptions) {
	_ = cmdName
	// Try to cast the default writer to JsonWriter
	jw, ok := opts.Writer.(*output.JsonWriter)
	wantsJson := (opts.Format == "json")

	// Global writer is set to JsonWriter, but this command wants to output
	// a different format. We have to close JsonWriter so that closing brackets
	// are printed and switch global writer to io.Writer
	if ok && !wantsJson {
		jw.Close()
		// Set global writer to default
		opts.Writer = os.Stdout
		opts.Writer = opts.GetOutputFileWriter()
		return
	}

	// Global writer is NOT JsonWriter, but this command wants to output
	// JSON, so we have to create new JsonWriter and set is as the global
	// writer
	if !ok && wantsJson {
		w := opts.GetOutputFileWriter()
		jw := output.NewDefaultJsonWriter(w, true)
		opts.Writer = jw
	}

	// Global writer is not JsonWriter and the command doesn't want to
	// output JSON, so we only have to set global writer to a default one
	if !ok && !wantsJson {
		opts.Writer = opts.GetOutputFileWriter()
	}
}

// InitJsonWriterApi inits JsonWriter for API responses
func InitJsonWriterApi(cmdName string, w io.Writer, opts *globals.GlobalOptions) {
	_ = cmdName
	_, ok := opts.Writer.(*output.JsonWriter)
	if opts.Format == "json" && !ok {
		jw := output.NewDefaultJsonWriter(w, false)
		jw.ShouldWriteMeta = true
		jw.GetMeta = func() (*types.MetaData, error) {
			chain := opts.Chain
			conn := rpc.TempConnection(chain)
			return conn.GetMetaData(opts.OutputOptions.TestMode)
		}
		opts.Writer = jw
	}
}

// CloseJsonWriterIfNeededApi will close JsonWriter if the format is json
func CloseJsonWriterIfNeededApi(cmdName string, err error, opts *globals.GlobalOptions) {
	_ = cmdName
	if opts.Format == "json" && err == nil {
		opts.Writer.(*output.JsonWriter).Close()
	}
}
